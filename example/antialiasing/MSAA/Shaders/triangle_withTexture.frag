#version 450 core

out vec4 FragColor;
in vec2 v_TexCoord;

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1,v_TexCoord).rgba;
}