#version 450 core

layout(location = 0) out vec4 fragColor;
layout(location = 1) out float count;

in vec4 v_color;
uniform vec4 i_showColor;

void main()
{
    fragColor = vec4(i_showColor);
    count = 1;
}




