#pragma once

#include <vector>
#include <Windows.h>

#include "BaseManager.h"

class ChairManager : public ej::BaseManager, public ej::PointerDefs<ChairManager>
{
public:
	ChairManager(const ej::Core& core);
	~ChairManager();

	void init(uint8_t portNumber);
	void close();

	void setOrientation(float pitch, float roll);

private:

	uint16_t CRC16(uint16_t crc, uint8_t b);
	uint16_t fullCRC(const std::vector<uint8_t>& data);

	std::vector<uint8_t> encode(std::vector<uint8_t> data);

	std::vector<uint8_t> prepare(std::vector<uint8_t> data);

	std::vector<uint8_t> floatToBytes(float x);

	bool m_isInitialized;

	HANDLE m_handle;
};