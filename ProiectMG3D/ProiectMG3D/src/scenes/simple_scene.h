#pragma once

#include "scenes/scene.h"

class SimpleScene : public Scene {
private:
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void ProcessInput(GLFWwindow* window);
	void CalculateDeltaTime();

	void CreateSceneContent();
public:
	SimpleScene(std::string scene_name);

	void Init();
	void StartFrame();
	void UpdateLogic();
	void EndFrame();
};
