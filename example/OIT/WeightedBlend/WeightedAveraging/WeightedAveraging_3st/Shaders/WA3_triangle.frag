#version 450 core

layout(location = 0) out vec4 accum;
layout(location = 1) out float reveal;

in vec4 v_color;
uniform vec4 i_showColor;

void main()
{
    
    float weight = 1.0;
    accum = vec4(i_showColor.rgb * i_showColor.a,i_showColor.a) * weight;
    reveal = i_showColor.a;  ///< 计算 1-alpha的累乘
}




