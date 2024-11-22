#version 430
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texture0;

void main()
{
	FragColor = vec4(0.5, 0.2, 0.3, 1.0);
}