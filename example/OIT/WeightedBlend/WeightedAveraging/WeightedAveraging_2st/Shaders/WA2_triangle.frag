#version 450 core

layout(location = 0) out vec4 fragColor;
layout(location = 1) out float count;
layout(location = 2) out float alpha;

in vec4 v_color;
uniform vec4 i_showColor;

void main()
{
    fragColor = vec4(i_showColor.rgb * i_showColor.a,i_showColor.a);
    count = 1.0;   ///< 计算像素的渲染次数
    alpha = i_showColor.a;  ///< 计算 1-alpha的累乘
}




