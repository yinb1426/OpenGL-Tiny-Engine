#version 430
in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec4 color0;

void main()
{
	vec3 textureColor = mix(texture(texture0, texCoords), texture(texture1, texCoords), texCoords.x).rgb;
	FragColor = vec4(textureColor, 1.0);
	// FragColor = color0;
}