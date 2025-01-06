#version 330 core
in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D albedo;
uniform float threshold;

void main()
{
    vec3 albedoColor = texture(albedo, texCoords).rgb;
    float brightness = dot(albedoColor, vec3(0.2126, 0.7152, 0.0722));
    vec3 bloomColor = (brightness > threshold) ? albedoColor : vec3(0.0);
    FragColor = vec4(bloomColor, 1.0);
}