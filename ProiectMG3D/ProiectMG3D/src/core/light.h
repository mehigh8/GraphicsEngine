#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class LightType {
	POINT = 0,
	SPOT
};

class Light {
public:
	LightType lightType;
	glm::vec3 color;

	// Spotlights only
	glm::vec3 direction;
	float angle;

	Light(glm::vec3 color); // Point light
	Light(glm::vec3 color, glm::vec3 direction, float angle); // Spotlight
};