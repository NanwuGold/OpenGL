#version 450 core
layout(location = 0) out vec4 vFragColor;

uniform sampler2D screenTexture;

in vec2 vTexCoord;

void main()
{
    vec4 color = texture(screenTexture,vTexCoord).rgba;
    vFragColor = color;
}

