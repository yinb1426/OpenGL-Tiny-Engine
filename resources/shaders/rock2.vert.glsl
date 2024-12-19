#version 430
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoords;
layout (location = 7) in mat4 aInstanceMatrix;

out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	texCoords = aTexCoords;
	gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 1.0);
}