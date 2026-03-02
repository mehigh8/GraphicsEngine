#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
private:
	std::string shaderID;
	GLuint programID;

	std::string LoadShaderSrc(std::string& shaderName);
public:
	std::string shaderName;

	Shader(std::string shaderID);
	~Shader();

	void Use();

	bool CreateProgram(std::string& shaderName);
	void SendUniformMat4(const GLchar* uniformName, glm::mat4 matrix);
	void SendUniformInt(const GLchar* uniformName, int value);
	void SendUniformFloat3(const GLchar* uniformName, glm::vec3 floats);
	void SendUniformFloat3Multiple(const GLchar* uniformName, GLsizei count, glm::vec3 floats[]);
	void SendUniformFloatMultiple(const GLchar* uniformName, GLsizei count, float floats[]);

	std::string GetShaderID();
	GLuint GetProgramID();
};
