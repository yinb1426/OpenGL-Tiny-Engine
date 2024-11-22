#version 430
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 texCoords;

layout(std140, binding = 0) uniform Matrices
{
	mat4 model;
	mat4 view;
	mat4 projection;
};

void main()
{
	texCoords = aTexCoords;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}