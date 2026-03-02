#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNor;
layout (location = 2) in vec2 vTex;
layout (location = 3) in vec3 vCol;

out vec4 color;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	color = vec4(vCol, 1.0);
	texCoord = vTex;
	gl_Position = projection * view * model * vec4(vPos, 1.0);
}