#version 450 core

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 count;

in vec4 v_color;
uniform vec4 i_showColor;

void main()
{
    fragColor = vec4(i_showColor.rgb * i_showColor.a,i_showColor.a);
    count.r = 1.0;

    /// TODO: 考虑计算累乘
    count.g = i_showColor.a;
}




