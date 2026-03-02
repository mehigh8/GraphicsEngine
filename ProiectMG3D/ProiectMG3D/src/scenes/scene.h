#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/camera.h"
#include "core/game_object.h"
#include "io/keyboard.h"
#include "io/mouse.h"
#include "ui/ui_element.h"
#include "ui/text.h"
#include "ui/slider.h"
#include "ui/input_text.h"
#include "ui/button.h"
#include "ui/dropdown.h"

#include "tinyxml2.h"

class Scene {
private:
	std::string scene_name;
public:
	static int screen_width;
	static int screen_height;
	glm::vec4 screen_bg_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	GLFWwindow* window;
	bool cursorEnabled;

	glm::vec3 global_light = glm::vec3(0.25f, 0.25f, 0.25f);

	Camera* mainCamera;

	std::unordered_map<std::string, Mesh*> meshes;
	std::unordered_map<std::string, Shader*> shaders;
	std::unordered_map<std::string, Texture*> textures;
	std::unordered_map<std::string, UIElement*> uiElements;
	std::unordered_map<std::string, GameObject*> gameObjects;

	double lastFrameTime;
	double deltaTime;

	Scene(std::string scene_name);

	std::string GetName();

	virtual void Init();
	virtual void Update();
	virtual void StartFrame();
	virtual void UpdateLogic() = 0;
	virtual void EndFrame();
	virtual void Terminate();

	virtual void LoadScene(const char* loadPath);
	virtual void SaveScene(const char* savePath);

	Text* GetText(std::string id);
	Slider* GetSlider(std::string id);
	InputText* GetInputText(std::string id);
	Button* GetButton(std::string id);
	Dropdown* GetDropdown(std::string id);
};
