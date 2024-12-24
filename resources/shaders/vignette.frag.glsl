#version 430 core
in vec2 texCoords;
out vec4 FragColor;

uniform vec4 vignetteColor;
uniform vec2 center;
uniform float intensity;
uniform float smoothness;
uniform sampler2D albedo;

void main()
{
   vec3 albedo = texture(albedo, texCoords).rgb;
   vec2 uv = texCoords - center;
   float borderLength = length(uv);
   borderLength = 1.0 - smoothstep(intensity - smoothness * 0.5, intensity + smoothness * 0.5, borderLength);

   FragColor = vec4(albedo * borderLength, 1.0);
}