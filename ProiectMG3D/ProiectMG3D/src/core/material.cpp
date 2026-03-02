#include "core/material.h"

Material::Material()
{
	ambient = glm::vec4(0);
	diffuse = glm::vec4(0);
	specular = glm::vec4(0);
	emissive = glm::vec4(0);
	shininess = 0;

	texture = NULL;
}