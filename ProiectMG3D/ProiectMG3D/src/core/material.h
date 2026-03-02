#pragma once

#include <glm/glm.hpp>

#include "core/texture.h"

class Material
{
public:
    Material();

    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    glm::vec4 emissive;
    float shininess;

    // Color texture
    Texture* texture;
};