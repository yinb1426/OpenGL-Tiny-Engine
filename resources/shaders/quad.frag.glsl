#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D framebuffer;
void main()
{             
    FragColor = texture2D(framebuffer, TexCoords) * vec4(0.451, 0.254, 0.655, 1.0);
}
