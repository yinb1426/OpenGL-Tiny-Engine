#version 330 core
out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D diffuse1;
uniform vec4 color0;

void main()
{
    FragColor = texture(diffuse1, texCoords);
}