#version 450 core

layout(location = 0) out vec4 accum;
layout(location = 1) out float reveal;

in vec4 v_color;
uniform vec4 i_showColor;

void main()
{
    float weight = 1.0;  /// TODO: implement it
    weight = clamp(pow(min(1.0, i_showColor.a * 10.0) + 0.01, 3.0) * pow(1.0 - gl_FragCoord.z * 0.9, 3.0) * 1e8,1e-2,3e3);
    /// 此处的权重计算并未采用 论文中的方案 使用了其它的替代方法
    accum = vec4(i_showColor.rgb * i_showColor.a,i_showColor.a) * weight;
    reveal = i_showColor.a;  ///< 计算 1-alpha的累乘
}




