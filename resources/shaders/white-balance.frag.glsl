#version 430 core
in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D albedo;
uniform float temperature;
uniform float tint;

vec3 AdjustWhiteBalance(vec3 color, float temperature, float tint)
{
    float t1 = temperature / 100.0;
    float t2 = tint / 100.0;
    
    vec3 tempAdjust = vec3(1.0 + t1, 1.0, 1.0 - t1);
    vec3 tintAdjust = vec3(1.0, 1.0 + t2, 1.0);

    return color * tempAdjust * tintAdjust;
}

void main()
{
    vec3 albedoColor = texture(albedo, texCoords).rgb;
    FragColor = vec4(AdjustWhiteBalance(albedoColor, temperature, tint), 1.0);
}