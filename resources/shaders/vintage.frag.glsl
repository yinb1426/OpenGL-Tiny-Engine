#version 430 core
in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D albedo;

void main()
{
   vec4 albedo = texture(albedo, texCoords);
   FragColor = vec4(1.0 - albedo.rgb, 1.0);
}