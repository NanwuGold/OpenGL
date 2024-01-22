#version 450 core
layout(location = 0) out vec4 FragColor;

in vec4 inputColor;

void main()
{
    FragColor = vec4(0.0,1.0,0.0,1.0);
    FragColor = inputColor;

    /// 亮度计算
    float L = 0.213 * inputColor.r + 0.715 * inputColor.g + 0.072 * inputColor.b;
    FragColor.a = L;
}
