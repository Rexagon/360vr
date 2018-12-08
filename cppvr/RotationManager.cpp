#include "RotationManager.h"

#include <SFML/Network/Packet.hpp>

RotationManager::RotationManager(const ej::Core& core) :
	BaseManager(core)
{
	
}

RotationManager::~RotationManager()
{
	stop();
}

void RotationManager::start()
{
	if (m_isReceiving) {
		return;
	}

	m_socket.setBlocking(false);
	if (m_socket.bind(PORT) != sf::Socket::Status::Done) {
		printf("Unable to create server on port: %d\n", PORT);
		return;
	}

	printf("Listening on port %d\n", PORT);

	m_isReceiving = true;
	m_listenerThread = std::make_unique<std::thread>([this]() {
		sf::Packet data;
		sf::IpAddress sender;
		unsigned short port;

		while (m_isReceiving) {
			if (m_socket.receive(data, sender, port) == sf::Socket::Status::Done) {
				handlePacket(std::move(data));
			}
			else {
				std::this_thread::sleep_for(std::chrono::milliseconds(11));
			}
		}
	});
}

void RotationManager::stop()
{
	if (!m_isReceiving) {
		return;
	}

	m_isReceiving = false;

	m_listenerThread->join();
}

glm::vec3 RotationManager::getState() const
{
	return m_state;
}

void RotationManager::handlePacket(sf::Packet&& packet)
{
	uint64_t timestamp;
	int32_t azimuth;
	int32_t pitch;
	int32_t roll;

	bool isValid = 
		packet >> timestamp &&
		packet >> azimuth &&
		packet >> pitch && 
		packet >> roll;

	if (!isValid) {
		return;
	}

	m_state.x = 1.0f / 1000000.0f * pitch;
	//m_state.y = 1.0f / 1000000.0f * azimuth;
	m_state.z = 1.0f / 1000000.0f * roll;

	//printf("%llu\t%d\t%d\n", timestamp, pitch, roll);
}

