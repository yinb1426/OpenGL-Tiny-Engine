#version 330 core
in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D albedo;
uniform bool horizontal;

uniform float weights[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{             
    vec2 texOffset = 1.0 / textureSize(albedo, 0); // gets size of single texel
    vec3 result = texture(albedo, texCoords).rgb * weights[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(albedo, texCoords + vec2(texOffset.x * i, 0.0)).rgb * weights[i];
            result += texture(albedo, texCoords - vec2(texOffset.x * i, 0.0)).rgb * weights[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(albedo, texCoords + vec2(0.0, texOffset.y * i)).rgb * weights[i];
            result += texture(albedo, texCoords - vec2(0.0, texOffset.y * i)).rgb * weights[i];
        }
    }
    FragColor = vec4(result, 1.0);
}