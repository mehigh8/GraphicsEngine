#version 330 core

in vec4 color;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D tex;

void main()
{
    FragColor = texture2D(tex, texCoord);
    if (FragColor.a < 0.5)
        discard;
} 