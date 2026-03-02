#pragma once

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <string>

class Texture
{
public: 
	int width;
	int height;
	int channels;

	GLuint textureType;
	GLenum wrappingMode;
	GLenum minFilter;
	GLenum magFilter;
	GLenum colorMode;

	std::string filename;

	Texture(std::string textureName);
	~Texture();

	GLuint GetID();
	std::string GetName();
	void GenerateTexture();
	bool LoadTexture(const char* filename, bool flipTexture = true);
	void BindToActiveTexture(GLenum activeTexture);
	void Unbind();
private:
	GLuint id;
	std::string name;
};
