#version 430 core
in vec3 normal;
in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, texCoords);
}