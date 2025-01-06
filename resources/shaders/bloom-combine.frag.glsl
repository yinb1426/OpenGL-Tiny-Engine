#version 430 core

in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D albedoScene; // 原始场景
uniform sampler2D blurScene; // 模糊后的高亮区域
uniform float intensity; // Bloom强度

void main()
{
    vec3 sceneColor = texture(albedoScene, texCoords).rgb;
    vec3 bloomColor = texture(blurScene, texCoords).rgb;
    FragColor = vec4(sceneColor + bloomColor * intensity, 1.0); // 合并
}