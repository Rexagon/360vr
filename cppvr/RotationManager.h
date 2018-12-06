#pragma once

#include <SFML/Network/UdpSocket.hpp>

#include "BaseManager.h"
#include <thread>
#include <atomic>
#include <glm/vec3.hpp>

class RotationManager : public ej::BaseManager, public ej::PointerDefs<RotationManager>
{
public:
	RotationManager(const ej::Core& core);
	~RotationManager();

	void start();
	void stop();

	glm::vec3 getState() const;

private:
	static const unsigned short PORT = 17742;

	void handlePacket(sf::Packet&& packet);

	sf::UdpSocket m_socket;

	glm::vec3 m_state;

	std::atomic_bool m_isReceiving{};
	std::unique_ptr<std::thread> m_listenerThread;
};
