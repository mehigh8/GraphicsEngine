#pragma once

#include "core/mesh.h"
#include "core/shader.h"
#include "core/light.h"

class GameObject {
private:
	std::string objectID;
public:
	// Transform
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	Mesh* mesh;
	Material* material;
	Light* light;
	Shader* shader;

	glm::mat4 model;

	GameObject(std::string objectID);

	void CalculateModelMatrix();

	std::string GetObjectID();
};
