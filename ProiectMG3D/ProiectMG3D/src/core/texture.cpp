#include "core/texture.h"

#include <iostream>

Texture::Texture(std::string textureName)
{
	width = 0;
	height = 0;
	channels = 0;
	id = 0;
	textureType = GL_TEXTURE_2D;
	wrappingMode = GL_REPEAT;
	minFilter = GL_LINEAR_MIPMAP_LINEAR;
	magFilter = GL_LINEAR;
	colorMode = GL_RGBA;

	name = std::move(textureName);
}

Texture::~Texture()
{

}

GLuint Texture::GetID()
{
	return id;
}

std::string Texture::GetName()
{
	return name;
}

void Texture::GenerateTexture()
{
	glGenTextures(1, &id);
	glBindTexture(textureType, id);

	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, wrappingMode);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, wrappingMode);
}

bool Texture::LoadTexture(const char* filename, bool flipTexture)
{
	this->filename = std::string(filename);
	if (flipTexture)
		stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
	if (data == NULL) {
		std::cout << "ERROR: Failed to load image at: " << filename << std::endl;
		return false;
	}

	switch (channels) {
	case 1:
		colorMode = GL_RED;
		break;
	case 2:
		colorMode = GL_RG;
		break;
	case 3:
		colorMode = GL_RGB;
		break;
	case 4:
		colorMode = GL_RGBA;
		break;
	default:
		colorMode = GL_RGBA;
	}

	//std::cout << "Width: " << width << " Height: " << height << " Channels: " << channels << " Format: " << colorMode << std::endl;

	GenerateTexture();
	glTexImage2D(textureType, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(textureType);
	
	Unbind();

	stbi_image_free(data);

	return true;
}

void Texture::BindToActiveTexture(GLenum activeTexture)
{
	glActiveTexture(activeTexture);
	glBindTexture(textureType, id);
}

void Texture::Unbind()
{
	glBindTexture(textureType, 0);
}