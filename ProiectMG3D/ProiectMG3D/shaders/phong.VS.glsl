#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNor;
layout (location = 2) in vec2 vTex;
layout (location = 3) in vec3 vCol;

out vec3 world_position;
out vec3 world_normal;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	world_position = (model * vec4(vPos, 1.0)).xyz;
	world_normal = normalize(mat3(model) * vNor);
	texCoord = vTex;

	gl_Position = projection * view * model * vec4(vPos, 1.0);
}