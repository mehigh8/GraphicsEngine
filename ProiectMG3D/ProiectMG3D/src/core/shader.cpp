#include "core/shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>

Shader::Shader(std::string shaderID)
{
	this->shaderID = std::move(shaderID);
	programID = 0;
}

Shader::~Shader()
{
	glDeleteProgram(programID);
}

void Shader::Use()
{
	if (programID == 0)
		return;

	glUseProgram(programID);
}

std::string Shader::LoadShaderSrc(std::string& shaderName)
{
	std::ifstream file;
	std::stringstream buf;

	std::string ret = "";

	file.open(shaderName);

	if (file.is_open()) {
		buf << file.rdbuf();
		ret = buf.str();
	}
	else {
		std::cout << "Could not open " << shaderName << std::endl;
	}

	file.close();

	return ret;
}

bool Shader::CreateProgram(std::string& shaderName)
{
	int success;
	char infoLog[512];

	this->shaderName = std::move(shaderName);

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertexShaderName = this->shaderName + ".VS.glsl";

	std::string vertexShaderSrcString = LoadShaderSrc(vertexShaderName);
	const GLchar* vertexShaderSrc = vertexShaderSrcString.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "VERTEX SHADER COMPILATION FAILED\n" << infoLog << std::endl;
		return false;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragmentShaderName = this->shaderName + ".FS.glsl";

	std::string fragmentShaderSrcString = LoadShaderSrc(fragmentShaderName);
	const GLchar* fragmentShaderSrc = fragmentShaderSrcString.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "FRAGMENT SHADER COMPILATION FAILED\n" << infoLog << std::endl;
		return false;
	}

	programID = glCreateProgram();

	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);

	glGetShaderiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(programID, 512, NULL, infoLog);
		std::cout << "SHADER PROGRAM LINK FAILED\n" << infoLog << std::endl;
		return false;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}

void Shader::SendUniformMat4(const GLchar* uniformName, glm::mat4 matrix)
{
	unsigned int loc = glGetUniformLocation(programID, uniformName);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SendUniformInt(const GLchar* uniformName, int value)
{
	unsigned int loc = glGetUniformLocation(programID, uniformName);
	glUniform1i(loc, value);
}

void Shader::SendUniformFloat3(const GLchar* uniformName, glm::vec3 floats)
{
	unsigned int loc = glGetUniformLocation(programID, uniformName);
	glUniform3f(loc, floats.x, floats.y, floats.z);
}

void Shader::SendUniformFloat3Multiple(const GLchar* uniformName, GLsizei count, glm::vec3 floats[])
{
	unsigned int loc = glGetUniformLocation(programID, uniformName);
	glUniform3fv(loc, count, glm::value_ptr(floats[0]));
}

void Shader::SendUniformFloatMultiple(const GLchar* uniformName, GLsizei count, float floats[])
{
	unsigned int loc = glGetUniformLocation(programID, uniformName);
	glUniform1fv(loc, count, floats);
}

GLuint Shader::GetProgramID()
{
	return programID;
}

std::string Shader::GetShaderID()
{
	return shaderID;
}
