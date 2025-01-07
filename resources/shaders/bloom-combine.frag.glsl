#version 430 core

in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D albedoScene;
uniform sampler2D blurScene;
uniform float intensity;

void main()
{
    vec3 sceneColor = texture(albedoScene, texCoords).rgb;
    vec3 bloomColor = texture(blurScene, texCoords).rgb;
    FragColor = vec4(sceneColor + bloomColor * intensity, 1.0);
}