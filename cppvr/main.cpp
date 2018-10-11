#include <memory>
#include <iostream>

#include <OpenVR/openvr.h>
#include <SFML/Graphics.hpp>

class VRSystem
{
public:
	VRSystem() :
		m_system(nullptr)
	{
		vr::HmdError error;
		vr::EVRApplicationType applicationType = vr::EVRApplicationType::VRApplication_Scene;
		m_system = vr::VR_Init(&error, applicationType);

		if (m_system == nullptr) {
			std::cout << "Error occured: " << error << std::endl;
			return;
		}

		vr::EVRInitError interfaceInitError;
		vr::IVRRenderModels* renderModelsInterface = static_cast<vr::IVRRenderModels*>(vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &interfaceInitError));
		if (renderModelsInterface == nullptr) {
			std::cout << "Interface error occured: " << interfaceInitError << std::endl;
			return;
		}

		char* buffer = new char[256];
		uint32_t modelRenderCount = renderModelsInterface->GetRenderModelCount();
		for (uint32_t i = 0; i < modelRenderCount; ++i) {
			renderModelsInterface->GetRenderModelName(i, buffer, 256);

			std::cout << buffer << std::endl;
		}
		delete[] buffer;
	}

	~VRSystem()
	{
		vr::VR_Shutdown();
	}

private:
	vr::IVRSystem* m_system;
};

int main()
{
	std::unique_ptr<VRSystem> vrSystem;
	if (vr::VR_IsHmdPresent()) {
		vrSystem = std::make_unique<VRSystem>();
	}
	else {
		std::cout << "" << std::endl;
	}

	sf::ContextSettings settings;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	sf::RenderWindow window(sf::VideoMode(800, 600), "CVR", sf::Style::Default, settings);

	sf::Clock timer;
	while (window.isOpen()) {
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}

		float dt = timer.restart().asSeconds();

		window.clear(sf::Color(30, 30, 30));
		window.display();
	}

	return 0;
}