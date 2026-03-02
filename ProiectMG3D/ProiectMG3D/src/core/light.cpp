#include "core/light.h"

Light::Light(glm::vec3 color)
{
	lightType = LightType::POINT;
	this->color = color;
}

Light::Light(glm::vec3 color, glm::vec3 direction, float angle)
{
	lightType = LightType::SPOT;
	this->color = color;
	this->direction = direction;
	this->angle = angle;
}
