#include "ChairManager.h"

#include <string>
#include <stdexcept>

ChairManager::ChairManager(const ej::Core& core) :
	ej::BaseManager(core), m_isInitialized(false)
{
}

ChairManager::~ChairManager()
{
	close();
}

void ChairManager::init(uint8_t portNumber)
{
	if (m_isInitialized) {
		return;
	}

	// Create handle

	std::string portName = "COM" + std::to_string(portNumber);
	m_handle = CreateFileA(portName.data(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (m_handle == INVALID_HANDLE_VALUE) {
		switch (GetLastError()) {
		case ERROR_FILE_NOT_FOUND:
			throw std::runtime_error(portName + " is not available");
		default:
			throw std::runtime_error("Unknown error while opening " + portName);
		}
	}

	// Init port

	DCB dcbSerialParameters = { 0 };

	if (!GetCommState(m_handle, &dcbSerialParameters)) {
		throw std::runtime_error("Unable to get serial parameters");
	}

	dcbSerialParameters.BaudRate = CBR_115200;
	dcbSerialParameters.ByteSize = 8;
	dcbSerialParameters.StopBits = ONESTOPBIT;
	dcbSerialParameters.Parity = NOPARITY;
	dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

	if (!SetCommState(m_handle, &dcbSerialParameters)) {
		throw std::runtime_error("Unable to set serial parameters");
	}

	PurgeComm(m_handle, PURGE_RXCLEAR | PURGE_TXCLEAR);

	m_isInitialized = true;
}

void ChairManager::close()
{
	if (m_isInitialized) {
		CloseHandle(m_handle);
		printf("Closing\n");
		m_isInitialized = false;
	}
}

void ChairManager::setOrientation(float pitch, float roll)
{
	if (!m_isInitialized) {
		return;
	}

	float yaw = 0.0f;

	std::vector<uint8_t> packet;
	packet.push_back(33);
	packet.push_back(12);
	packet.push_back(3);

	for (int i = 0; i < 4; ++i) {
		packet.push_back(*(reinterpret_cast<const uint8_t*>(&pitch) + i));
	}
	for (int i = 0; i < 4; ++i) {
		packet.push_back(*(reinterpret_cast<const uint8_t*>(&roll) + i));
	}
	for (int i = 0; i < 4; ++i) {
		packet.push_back(*(reinterpret_cast<const uint8_t*>(&yaw) + i));
	}

	packet = encode(packet);

	DWORD byteSend;
	if (!WriteFile(m_handle, packet.data(), static_cast<DWORD>(packet.size()), &byteSend, 0)) {
		printf("Unable to send packet\n");
		//return false;
	}
}

uint16_t ChairManager::CRC16(uint16_t crc, uint8_t b)
{
	uint16_t num1 = 255 & (crc >> 8 ^ b);
	uint16_t num2 = num1 ^ num1 >> 4;
	return (crc ^ num2 << 4 ^ num2 >> 3) << 8 ^ (num2 ^ num2 << 5) & 255;
}

uint16_t ChairManager::fullCRC(const std::vector<uint8_t>& data)
{
	uint16_t crc = 58005;
	for (int i = 0; i <= data.size() - 1; ++i) {
		crc = CRC16(crc, data[i]);
	}
	return crc;
}

std::vector<uint8_t> ChairManager::encode(std::vector<uint8_t> data)
{
	const auto crc = fullCRC(data);
	data.push_back(crc & 255);
	data.push_back(crc >> 8);
	const auto payload = prepare(data);

	std::vector<uint8_t> result;
	result.resize(payload.size() + 2);

	result.front() = 255;
	std::memcpy(&result[1], payload.data(), payload.size());
	result.back() = 254;

	return result;
}

std::vector<uint8_t> ChairManager::prepare(std::vector<uint8_t> data)
{
	std::vector<uint8_t> result;
	for (size_t i = 0; i < data.size(); ++i) {
		if (data[i] >= 253) {
			result.push_back(253);
			result.push_back(data[i] - 253);
		}
		else {
			result.push_back(data[i]);
		}
	}
	return result;
}
