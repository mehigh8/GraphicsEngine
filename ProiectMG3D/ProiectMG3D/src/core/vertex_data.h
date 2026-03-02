#pragma once

#include <glm/glm.hpp>

struct VertexData
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 color;

	VertexData(glm::vec3 position, glm::vec3 color = glm::vec3(1), glm::vec3 normal = glm::vec3(0, 1, 0), glm::vec2 texCoords = glm::vec2(0))
	{
		this->position = position;
		this->color = color;
		this->normal = normal;
		this->texCoords = texCoords;
	}
};