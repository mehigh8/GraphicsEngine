#include "scenes/scene.h"
#include <cstdio>

using namespace tinyxml2;

int Scene::screen_height = 720;
int Scene::screen_width = 1280;

Scene::Scene(std::string scene_name)
{
	this->scene_name = std::move(scene_name);
}

std::string Scene::GetName()
{
	return scene_name;
}

void Scene::Init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Scene::Update()
{
	while (!glfwWindowShouldClose(window))
	{
		StartFrame();
		UpdateLogic();
		EndFrame();
	}
}

void Scene::StartFrame()
{
	glClearColor(screen_bg_color.r, screen_bg_color.g, screen_bg_color.b, screen_bg_color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::EndFrame()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Scene::Terminate()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}

Text* Scene::GetText(std::string id)
{
	return (Text*)uiElements[id];
}

Slider* Scene::GetSlider(std::string id)
{
	return (Slider*)uiElements[id];
}

InputText* Scene::GetInputText(std::string id)
{
	return (InputText*)uiElements[id];
}

Button* Scene::GetButton(std::string id)
{
	return (Button*)uiElements[id];
}

Dropdown* Scene::GetDropdown(std::string id)
{
	return (Dropdown*)uiElements[id];
}

void Scene::LoadScene(const char *loadPath)
{
	XMLDocument doc;
	doc.LoadFile(loadPath);

	XMLElement* sceneElement = doc.FirstChildElement("Scene");
	sscanf_s(sceneElement->Attribute("screen_resolution"), "%d %d", &screen_width, &screen_height);

	screen_bg_color = glm::vec4(0.0f);
	sscanf_s(sceneElement->Attribute("screen_bg_color"), "%f %f %f %f", &screen_bg_color.r, &screen_bg_color.g, &screen_bg_color.b, &screen_bg_color.a);
	
	if (sceneElement->Attribute("global_light")) {
		sscanf_s(sceneElement->Attribute("global_light"), "%f %f %f", &global_light.r, &global_light.g, &global_light.b);
	}

	XMLElement* cameraElement = sceneElement->FirstChildElement("Camera");

	mainCamera = new Camera(glm::vec3(0.0f));
	sscanf_s(cameraElement->Attribute("position"), "%f %f %f", &mainCamera->cameraPos.x, &mainCamera->cameraPos.y, &mainCamera->cameraPos.z);
	sscanf_s(cameraElement->Attribute("orientation"), "%f %f", &mainCamera->yaw, &mainCamera->pitch);
	mainCamera->fov = cameraElement->FloatAttribute("fov");
	mainCamera->zNear = cameraElement->FloatAttribute("zNear");
	mainCamera->zFar = cameraElement->FloatAttribute("zFar");

	mainCamera->Rotate(0.0f, 0.0f); // This is called to force camera to update internal vectors

	XMLElement* meshesElement = sceneElement->FirstChildElement("Meshes");
	for (XMLElement* meshElement = meshesElement->FirstChildElement("Mesh"); meshElement; meshElement = meshElement->NextSiblingElement("Mesh"))
	{
		Mesh* mesh = new Mesh(meshElement->Attribute("meshID"));

		if (meshElement->Attribute("directory"))
		{
			mesh->Load(meshElement->Attribute("directory"), meshElement->Attribute("file"));
		}
		else
		{
			std::vector<VertexData> vertices;
			for (XMLElement* vertexElement = meshElement->FirstChildElement("Vertex"); vertexElement; vertexElement = vertexElement->NextSiblingElement("Vertex"))
			{
				glm::vec3 pos = glm::vec3(0.0f);
				glm::vec3 nor = glm::vec3(0.0f);
				glm::vec2 tex = glm::vec2(0.0f);
				glm::vec3 col = glm::vec3(0.0f);

				sscanf_s(vertexElement->Attribute("position"), "%f %f %f", &pos.x, &pos.y, &pos.z);
				sscanf_s(vertexElement->Attribute("normal"), "%f %f %f", &nor.x, &nor.y, &nor.z);
				sscanf_s(vertexElement->Attribute("texCoords"), "%f %f", &tex.x, &tex.y);
				sscanf_s(vertexElement->Attribute("color"), "%f %f %f", &col.x, &col.y, &col.z);

				vertices.push_back(VertexData(pos, col, nor, tex));
			}

			std::vector<unsigned int> indices;
			for (XMLElement* indexElement = meshElement->FirstChildElement("Index"); indexElement; indexElement = indexElement->NextSiblingElement("Index"))
			{
				unsigned int i1, i2, i3;
				sscanf_s(indexElement->Attribute("indices"), "%d %d %d", &i1, &i2, &i3);
				indices.push_back(i1);
				indices.push_back(i2);
				indices.push_back(i3);
			}
			mesh->Init(vertices, indices);
		}

		meshes[mesh->GetMeshID()] = mesh;
	}

	XMLElement* shadersElement = sceneElement->FirstChildElement("Shaders");
	for (XMLElement* shaderElement = shadersElement->FirstChildElement("Shader"); shaderElement; shaderElement = shaderElement->NextSiblingElement("Shader"))
	{
		Shader* shader = new Shader(shaderElement->Attribute("shaderID"));
		std::string shaderPath = shaderElement->Attribute("path");
		shader->CreateProgram(shaderPath);
		shaders[shader->GetShaderID()] = shader;
	}

	XMLElement* texturesElement = sceneElement->FirstChildElement("Textures");
	for (XMLElement* textureElement = texturesElement->FirstChildElement("Texture"); textureElement; textureElement = textureElement->NextSiblingElement("Texture"))
	{
		Texture* texture = new Texture(textureElement->Attribute("textureID"));
		texture->LoadTexture(textureElement->Attribute("path"));
		textures[texture->GetName()] = texture;
	}

	XMLElement* uiElementsElement = sceneElement->FirstChildElement("UIElements");
	for (XMLElement* uiElement = uiElementsElement->FirstChildElement("UIElement"); uiElement; uiElement = uiElement->NextSiblingElement("UIElement"))
	{
		std::string uiType = uiElement->Attribute("uiType");
		ImVec2 uiPos = ImVec2();
		sscanf_s(uiElement->Attribute("position"), "%f %f", &uiPos.x, &uiPos.y);

		if (uiType == "text")
		{
			ImVec4 color = ImVec4();
			sscanf_s(uiElement->Attribute("color"), "%f %f %f %f", &color.x, &color.y, &color.z, &color.w);

			Text* text = new Text(uiElement->Attribute("elementID"), uiPos, uiElement->Attribute("text"), color, uiElement->FloatAttribute("fontSize"));
			uiElements[text->GetID()] = text;
		}

		if (uiType == "slider")
		{
			ImVec2 size = ImVec2();
			ImVec4 bgColor = ImVec4();
			ImVec4 fillColor = ImVec4();
			sscanf_s(uiElement->Attribute("size"), "%f %f", &size.x, &size.y);
			sscanf_s(uiElement->Attribute("bgColor"), "%f %f %f %f", &bgColor.x, &bgColor.y, &bgColor.z, &bgColor.w);
			sscanf_s(uiElement->Attribute("fillColor"), "%f %f %f %f", &fillColor.x, &fillColor.y, &fillColor.z, &fillColor.w);

			Slider* slider = new Slider(uiElement->Attribute("elementID"), uiPos, size, uiElement->FloatAttribute("value"), bgColor, fillColor);
			uiElements[slider->GetID()] = slider;
		}

		if (uiType == "inputText")
		{
			ImVec4 bgColor = ImVec4();
			ImVec4 textColor = ImVec4();
			sscanf_s(uiElement->Attribute("bgColor"), "%f %f %f %f", &bgColor.x, &bgColor.y, &bgColor.z, &bgColor.w);
			sscanf_s(uiElement->Attribute("textColor"), "%f %f %f %f", &textColor.x, &textColor.y, &textColor.z, &textColor.w);

			InputText* inputText = new InputText(uiElement->Attribute("elementID"), uiPos, uiElement->FloatAttribute("size"), bgColor, textColor, uiElement->Attribute("text"));
			uiElements[inputText->GetID()] = inputText;
		}

		if (uiType == "button")
		{
			ImVec2 size = ImVec2();
			sscanf_s(uiElement->Attribute("size"), "%f %f", &size.x, &size.y);

			Button* button = new Button(uiElement->Attribute("elementID"), uiPos, size, uiElement->Attribute("text"), nullptr);
			
			ImVec4 normalColor = ImVec4();
			ImVec4 hoveredColor = ImVec4();
			ImVec4 activeColor = ImVec4();
			ImVec4 textColor = ImVec4();

			sscanf_s(uiElement->Attribute("normalColor"), "%f %f %f %f", &normalColor.x, &normalColor.y, &normalColor.z, &normalColor.w);
			sscanf_s(uiElement->Attribute("hoveredColor"), "%f %f %f %f", &hoveredColor.x, &hoveredColor.y, &hoveredColor.z, &hoveredColor.w);
			sscanf_s(uiElement->Attribute("activeColor"), "%f %f %f %f", &activeColor.x, &activeColor.y, &activeColor.z, &activeColor.w);
			sscanf_s(uiElement->Attribute("textColor"), "%f %f %f %f", &textColor.x, &textColor.y, &textColor.z, &textColor.w);

			button->ChangeColors(normalColor, hoveredColor, activeColor, textColor);
			uiElements[button->GetID()] = button;
		}

		if (uiType == "dropdown")
		{
			Dropdown* dropdown = new Dropdown(uiElement->Attribute("elementID"), uiPos, uiElement->FloatAttribute("size"));

			ImVec4 bgColor = ImVec4();
			ImVec4 hoverColor = ImVec4();
			ImVec4 activeColor = ImVec4();
			ImVec4 popupColor = ImVec4();
			ImVec4 textColor = ImVec4();

			sscanf_s(uiElement->Attribute("bgColor"), "%f %f %f %f", &bgColor.x, &bgColor.y, &bgColor.z, &bgColor.w);
			sscanf_s(uiElement->Attribute("hoverColor"), "%f %f %f %f", &hoverColor.x, &hoverColor.y, &hoverColor.z, &hoverColor.w);
			sscanf_s(uiElement->Attribute("activeColor"), "%f %f %f %f", &activeColor.x, &activeColor.y, &activeColor.z, &activeColor.w);
			sscanf_s(uiElement->Attribute("popupColor"), "%f %f %f %f", &popupColor.x, &popupColor.y, &popupColor.z, &popupColor.w);
			sscanf_s(uiElement->Attribute("textColor"), "%f %f %f %f", &textColor.x, &textColor.y, &textColor.z, &textColor.w);

			dropdown->ChangeColors(bgColor, hoverColor, activeColor, popupColor, textColor);

			for (XMLElement* itemElement = uiElement->FirstChildElement("DropdownItem"); itemElement; itemElement = itemElement->NextSiblingElement("DropdownItem"))
				dropdown->AddItem(itemElement->Attribute("item"));

			dropdown->currentItem = uiElement->IntAttribute("currentItem");

			uiElements[dropdown->GetID()] = dropdown;
		}
	}

	XMLElement* gameObjectsElement = sceneElement->FirstChildElement("GameObjects");
	for (XMLElement* gameObjectElement = gameObjectsElement->FirstChildElement("GameObject"); gameObjectElement; gameObjectElement = gameObjectElement->NextSiblingElement("GameObject"))
	{
		GameObject* gameObject = new GameObject(gameObjectElement->Attribute("objectID"));

		XMLElement* transformElement = gameObjectElement->FirstChildElement("Transform");
		sscanf_s(transformElement->Attribute("position"), "%f %f %f", &gameObject->position.x, &gameObject->position.y, &gameObject->position.z);
		sscanf_s(transformElement->Attribute("rotation"), "%f %f %f", &gameObject->rotation.x, &gameObject->rotation.y, &gameObject->rotation.z);
		sscanf_s(transformElement->Attribute("scale"), "%f %f %f", &gameObject->scale.x, &gameObject->scale.y, &gameObject->scale.z);

		gameObject->mesh = meshes[gameObjectElement->Attribute("mesh")];
		gameObject->shader = shaders[gameObjectElement->Attribute("shader")];

		if (XMLElement* materialElement = gameObjectElement->FirstChildElement("Material")) {
			Material* material = new Material();
			sscanf_s(materialElement->Attribute("ambient"), "%f %f %f %f", &material->ambient.x, &material->ambient.y, &material->ambient.z, &material->ambient.w);
			sscanf_s(materialElement->Attribute("diffuse"), "%f %f %f %f", &material->diffuse.x, &material->diffuse.y, &material->diffuse.z, &material->diffuse.w);
			sscanf_s(materialElement->Attribute("specular"), "%f %f %f %f", &material->specular.x, &material->specular.y, &material->specular.z, &material->specular.w);
			sscanf_s(materialElement->Attribute("emissive"), "%f %f %f %f", &material->emissive.x, &material->emissive.y, &material->emissive.z, &material->emissive.w);
			material->shininess = materialElement->FloatAttribute("shininess");
			if (const char* texture = materialElement->Attribute("texture")) {
				material->texture = textures[texture];
			}

			gameObject->material = material;
		}

		if (XMLElement* lightElement = gameObjectElement->FirstChildElement("Light")) {
			glm::vec3 color = glm::vec3(0.0f);
			sscanf_s(lightElement->Attribute("color"), "%f %f %f", &color.x, &color.y, &color.z);
			std::string lightType = lightElement->Attribute("lightType");

			if (lightType == "Point")
			{
				Light* light = new Light(color);

				gameObject->light = light;
			}
			else
			{
				glm::vec3 direction = glm::vec3(0.0f);
				sscanf_s(lightElement->Attribute("direction"), "%f %f %f", &direction.x, &direction.y, &direction.z);

				Light* light = new Light(color, direction, lightElement->FloatAttribute("angle"));

				gameObject->light = light;
			}
		}

		gameObjects[gameObject->GetObjectID()] = gameObject;
	}
}

void Scene::SaveScene(const char *savePath)
{
	XMLDocument doc;
	// Scene
	XMLElement* sceneElement = doc.NewElement("Scene");
	sceneElement->SetAttribute("name", GetName().c_str());
	sceneElement->SetAttribute("screen_resolution", (std::to_string(screen_width) + " " +
													 std::to_string(screen_height)).c_str());
	sceneElement->SetAttribute("screen_bg_color", (std::to_string(screen_bg_color.r) + " " +
												   std::to_string(screen_bg_color.g) + " " +
												   std::to_string(screen_bg_color.b) + " " +
												   std::to_string(screen_bg_color.a)).c_str());
	sceneElement->SetAttribute("global_light", (std::to_string(global_light.r) + " " +
												std::to_string(global_light.g) + " " +
												std::to_string(global_light.b)).c_str());

	doc.InsertFirstChild(sceneElement);
	// Camera
	XMLElement* cameraElement = doc.NewElement("Camera");
	cameraElement->SetAttribute("position", (std::to_string(mainCamera->cameraPos.x) + " " +
											 std::to_string(mainCamera->cameraPos.y) + " " +
											 std::to_string(mainCamera->cameraPos.z)).c_str());
	cameraElement->SetAttribute("orientation", (std::to_string(mainCamera->yaw) + " " +
												std::to_string(mainCamera->pitch)).c_str());
	cameraElement->SetAttribute("fov", mainCamera->fov);
	cameraElement->SetAttribute("zNear", mainCamera->zNear);
	cameraElement->SetAttribute("zFar", mainCamera->zFar);

	sceneElement->InsertEndChild(cameraElement);

	// Meshes
	XMLElement* meshesElement = doc.NewElement("Meshes");
	meshesElement->SetAttribute("count", meshes.size());
	for (auto it = meshes.begin(); it != meshes.end(); it++)
	{
		XMLElement* meshElement = doc.NewElement("Mesh");
		meshElement->SetAttribute("meshID", it->first.c_str());
		if (it->second->IsImported())
		{
			meshElement->SetAttribute("directory", it->second->directory.c_str());
			meshElement->SetAttribute("file", it->second->file.c_str());
		}
		else
		{
			for (int i = 0; i < it->second->vertices.size(); i++)
			{
				XMLElement* vertexElement = doc.NewElement("Vertex");
				vertexElement->SetAttribute("position", (std::to_string(it->second->vertices[i].position.x) + " " +
														 std::to_string(it->second->vertices[i].position.y) + " " +
														 std::to_string(it->second->vertices[i].position.z)).c_str());
				vertexElement->SetAttribute("normal", (std::to_string(it->second->vertices[i].normal.x) + " " +
													   std::to_string(it->second->vertices[i].normal.y) + " " +
													   std::to_string(it->second->vertices[i].normal.z)).c_str());
				vertexElement->SetAttribute("texCoords", (std::to_string(it->second->vertices[i].texCoords.x) + " " +
														  std::to_string(it->second->vertices[i].texCoords.y)).c_str());
				vertexElement->SetAttribute("color", (std::to_string(it->second->vertices[i].color.x) + " " +
													  std::to_string(it->second->vertices[i].color.y) + " " +
													  std::to_string(it->second->vertices[i].color.z)).c_str());
				meshElement->InsertEndChild(vertexElement);
			}

			for (int i = 0; i < it->second->indices.size(); i += 3)
			{
				XMLElement* indexElement = doc.NewElement("Index");
				indexElement->SetAttribute("indices", (std::to_string(it->second->indices[i]) + " " +
													   std::to_string(it->second->indices[i+1]) + " " +
													   std::to_string(it->second->indices[i+2])).c_str());
				meshElement->InsertEndChild(indexElement);
			}
		}
		meshesElement->InsertEndChild(meshElement);
	}
	sceneElement->InsertEndChild(meshesElement);

	// Shaders
	XMLElement* shadersElement = doc.NewElement("Shaders");
	shadersElement->SetAttribute("count", shaders.size());
	for (auto it = shaders.begin(); it != shaders.end(); it++)
	{
		XMLElement* shaderElement = doc.NewElement("Shader");
		shaderElement->SetAttribute("shaderID", it->first.c_str());
		shaderElement->SetAttribute("path", it->second->shaderName.c_str());

		shadersElement->InsertEndChild(shaderElement);
	}
	sceneElement->InsertEndChild(shadersElement);

	// Textures
	XMLElement* texturesElement = doc.NewElement("Textures");
	texturesElement->SetAttribute("count", textures.size());
	for (auto it = textures.begin(); it != textures.end(); it++)
	{
		XMLElement* textureElement = doc.NewElement("Texture");
		textureElement->SetAttribute("textureID", it->first.c_str());
		textureElement->SetAttribute("path", it->second->filename.c_str());

		texturesElement->InsertEndChild(textureElement);
	}
	sceneElement->InsertEndChild(texturesElement);

	// UI
	XMLElement* uiElementsElement = doc.NewElement("UIElements");
	uiElementsElement->SetAttribute("count", uiElements.size());
	for (auto it = uiElements.begin(); it != uiElements.end(); it++)
	{
		XMLElement* uiElement = doc.NewElement("UIElement");
		uiElement->SetAttribute("elementID", it->first.c_str());
		uiElement->SetAttribute("position", (std::to_string(it->second->uiPos.x) + " " +
											 std::to_string(it->second->uiPos.y)).c_str());

		if (Text* text = dynamic_cast<Text*>(it->second)) // Text
		{
			uiElement->SetAttribute("uiType", "text");
			uiElement->SetAttribute("text", text->text.c_str());
			uiElement->SetAttribute("color", (std::to_string(text->color.x) + " " +
											  std::to_string(text->color.y) + " " +
											  std::to_string(text->color.z) + " " +
											  std::to_string(text->color.w)).c_str());
			uiElement->SetAttribute("fontSize", text->fontSize);
		}

		if (Slider* slider = dynamic_cast<Slider*>(it->second)) // Slider
		{
			uiElement->SetAttribute("uiType", "slider");
			uiElement->SetAttribute("size", (std::to_string(slider->size.x) + " " +
											 std::to_string(slider->size.y)).c_str());
			uiElement->SetAttribute("value", slider->value);
			uiElement->SetAttribute("bgColor", (std::to_string(slider->bgColor.x) + " " +
												std::to_string(slider->bgColor.y) + " " +
												std::to_string(slider->bgColor.z) + " " +
												std::to_string(slider->bgColor.w)).c_str());
			uiElement->SetAttribute("fillColor", (std::to_string(slider->fillColor.x) + " " +
												  std::to_string(slider->fillColor.y) + " " +
												  std::to_string(slider->fillColor.z) + " " +
												  std::to_string(slider->fillColor.w)).c_str());
		}

		if (InputText* inputText = dynamic_cast<InputText*>(it->second)) // Input Text
		{
			uiElement->SetAttribute("uiType", "inputText");
			uiElement->SetAttribute("size", inputText->size);
			uiElement->SetAttribute("text", inputText->inputBuffer);
			uiElement->SetAttribute("bgColor", (std::to_string(inputText->bgColor.x) + " " +
												std::to_string(inputText->bgColor.y) + " " +
												std::to_string(inputText->bgColor.z) + " " +
												std::to_string(inputText->bgColor.w)).c_str());
			uiElement->SetAttribute("textColor", (std::to_string(inputText->textColor.x) + " " +
												  std::to_string(inputText->textColor.y) + " " +
												  std::to_string(inputText->textColor.z) + " " +
												  std::to_string(inputText->textColor.w)).c_str());
		}

		if (Button* button = dynamic_cast<Button*>(it->second)) // Button
		{
			uiElement->SetAttribute("uiType", "button");
			uiElement->SetAttribute("size", (std::to_string(button->size.x) + " " +
											 std::to_string(button->size.y)).c_str());
			uiElement->SetAttribute("text", button->text.c_str());
			uiElement->SetAttribute("normalColor", (std::to_string(button->normalColor.x) + " " +
													std::to_string(button->normalColor.y) + " " +
													std::to_string(button->normalColor.z) + " " +
													std::to_string(button->normalColor.w)).c_str());
			uiElement->SetAttribute("hoveredColor", (std::to_string(button->hoveredColor.x) + " " +
													 std::to_string(button->hoveredColor.y) + " " +
													 std::to_string(button->hoveredColor.z) + " " +
													 std::to_string(button->hoveredColor.w)).c_str());
			uiElement->SetAttribute("activeColor", (std::to_string(button->activeColor.x) + " " +
													std::to_string(button->activeColor.y) + " " +
													std::to_string(button->activeColor.z) + " " +
													std::to_string(button->activeColor.w)).c_str());
			uiElement->SetAttribute("textColor", (std::to_string(button->textColor.x) + " " +
												  std::to_string(button->textColor.y) + " " +
												  std::to_string(button->textColor.z) + " " +
												  std::to_string(button->textColor.w)).c_str());
		}

		if (Dropdown* dropdown = dynamic_cast<Dropdown*>(it->second)) // Dropdown
		{
			uiElement->SetAttribute("uiType", "dropdown");
			uiElement->SetAttribute("size", dropdown->size);
			uiElement->SetAttribute("currentItem", dropdown->currentItem);
			uiElement->SetAttribute("bgColor", (std::to_string(dropdown->bgColor.x) + " " +
												std::to_string(dropdown->bgColor.y) + " " +
												std::to_string(dropdown->bgColor.z) + " " +
												std::to_string(dropdown->bgColor.w)).c_str());
			uiElement->SetAttribute("hoverColor", (std::to_string(dropdown->hoverColor.x) + " " +
												   std::to_string(dropdown->hoverColor.y) + " " +
												   std::to_string(dropdown->hoverColor.z) + " " +
												   std::to_string(dropdown->hoverColor.w)).c_str());
			uiElement->SetAttribute("activeColor", (std::to_string(dropdown->activeColor.x) + " " +
													std::to_string(dropdown->activeColor.y) + " " +
													std::to_string(dropdown->activeColor.z) + " " +
													std::to_string(dropdown->activeColor.w)).c_str());
			uiElement->SetAttribute("popupColor", (std::to_string(dropdown->popupColor.x) + " " +
												   std::to_string(dropdown->popupColor.y) + " " +
												   std::to_string(dropdown->popupColor.z) + " " +
												   std::to_string(dropdown->popupColor.w)).c_str());
			uiElement->SetAttribute("textColor", (std::to_string(dropdown->textColor.x) + " " +
												  std::to_string(dropdown->textColor.y) + " " +
												  std::to_string(dropdown->textColor.z) + " " +
												  std::to_string(dropdown->textColor.w)).c_str());
			
			for (int i = 0; i < dropdown->items.size(); i++)
			{
				XMLElement* itemElement = doc.NewElement("DropdownItem");
				itemElement->SetAttribute("item", dropdown->items[i].c_str());

				uiElement->InsertEndChild(itemElement);
			}
		}

		uiElementsElement->InsertEndChild(uiElement);
	}
	sceneElement->InsertEndChild(uiElementsElement);

	// GameObjects
	XMLElement* gameObjectsElement = doc.NewElement("GameObjects");
	gameObjectsElement->SetAttribute("count", gameObjects.size());
	for (auto it = gameObjects.begin(); it != gameObjects.end(); it++)
	{
		XMLElement* gameObjectElement = doc.NewElement("GameObject");
		gameObjectElement->SetAttribute("objectID", it->first.c_str());

		XMLElement* transformElement = doc.NewElement("Transform");
		transformElement->SetAttribute("position", (std::to_string(it->second->position.x) + " " +
													std::to_string(it->second->position.y) + " " +
													std::to_string(it->second->position.z)).c_str());
		transformElement->SetAttribute("rotation", (std::to_string(it->second->rotation.x) + " " +
													std::to_string(it->second->rotation.y) + " " +
													std::to_string(it->second->rotation.z)).c_str());
		transformElement->SetAttribute("scale", (std::to_string(it->second->scale.x) + " " +
												 std::to_string(it->second->scale.y) + " " +
												 std::to_string(it->second->scale.z)).c_str());
		gameObjectElement->InsertEndChild(transformElement);

		gameObjectElement->SetAttribute("mesh", it->second->mesh->GetMeshID().c_str());
		gameObjectElement->SetAttribute("shader", it->second->shader->GetShaderID().c_str());

		if (it->second->material) {
			XMLElement* materialElement = doc.NewElement("Material");
			materialElement->SetAttribute("ambient", (std::to_string(it->second->material->ambient.x) + " " +
				std::to_string(it->second->material->ambient.y) + " " +
				std::to_string(it->second->material->ambient.z) + " " +
				std::to_string(it->second->material->ambient.w)).c_str());
			materialElement->SetAttribute("diffuse", (std::to_string(it->second->material->diffuse.x) + " " +
				std::to_string(it->second->material->diffuse.y) + " " +
				std::to_string(it->second->material->diffuse.z) + " " +
				std::to_string(it->second->material->diffuse.w)).c_str());
			materialElement->SetAttribute("specular", (std::to_string(it->second->material->specular.x) + " " +
				std::to_string(it->second->material->specular.y) + " " +
				std::to_string(it->second->material->specular.z) + " " +
				std::to_string(it->second->material->specular.w)).c_str());
			materialElement->SetAttribute("emissive", (std::to_string(it->second->material->emissive.x) + " " +
				std::to_string(it->second->material->emissive.y) + " " +
				std::to_string(it->second->material->emissive.z) + " " +
				std::to_string(it->second->material->emissive.w)).c_str());
			materialElement->SetAttribute("shininess", it->second->material->shininess);
			if (it->second->material->texture)
				materialElement->SetAttribute("texture", it->second->material->texture->GetName().c_str());

			gameObjectElement->InsertEndChild(materialElement);
		}

		if (it->second->light)
		{
			XMLElement* lightElement = doc.NewElement("Light");
			lightElement->SetAttribute("lightType", it->second->light->lightType == LightType::POINT ? "Point" : "Spot");
			lightElement->SetAttribute("color", (std::to_string(it->second->light->color.x) + " " +
												 std::to_string(it->second->light->color.y) + " " +
												 std::to_string(it->second->light->color.z)).c_str());
			if (it->second->light->lightType == LightType::SPOT)
			{
				lightElement->SetAttribute("direction", (std::to_string(it->second->light->direction.x) + " " +
														 std::to_string(it->second->light->direction.y) + " " +
														 std::to_string(it->second->light->direction.z)).c_str());
				lightElement->SetAttribute("angle", it->second->light->angle);
			}

			gameObjectElement->InsertEndChild(lightElement);
		}

		gameObjectsElement->InsertEndChild(gameObjectElement);
	}
	sceneElement->InsertEndChild(gameObjectsElement);

	doc.SaveFile(savePath);
}
