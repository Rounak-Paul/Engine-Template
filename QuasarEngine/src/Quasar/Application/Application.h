#pragma once
#include "qspch.h"

#include <Quasar/Core.h>

#include <Quasar/RenderSystem/Model.h>
#include <Quasar/RenderSystem/RenderSystem.h>
#include <Quasar/RenderSystem/Buffer.h>
#include <Quasar/RenderSystem/FrameInfo.h>
#include <Quasar/GameObject/GameObject.h>

namespace Quasar {

	/// <summary>
	///		<para>
	///			Quasar Application
	///		</para>
	///		<para>
	///			Main application to instantiate Quasae Engine. Performs main application loop. 
	///		</para>
	/// </summary>
	class QUASAR_API Application
	{
	public:
		Application();
		virtual ~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		void Run();

		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

	private:
		void LoadGameObjects();

		void FPS(int& frames, std::chrono::time_point<std::chrono::high_resolution_clock>& startTime);

		Window window{ WIDTH, HEIGHT, "QuasarEngine" };
		Device device{ window };
		Renderer renderer{ window, device };

		std::vector<GameObject> gameObjects;
	};

	// To be defined in Client
	Application* CreateApplication();

}


