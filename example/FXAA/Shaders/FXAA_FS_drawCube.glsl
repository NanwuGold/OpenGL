#version 450 core
layout(location = 0) out vec4 FragColor;

in vec4 inputColor;

void main()
{
    FragColor = vec4(0.0,1.0,0.0,1.0);
    FragColor = vec4(0.0,1.0,1.0,1.0);

    /// 亮度计算
    float L = 0.213 * FragColor.r + 0.715 * FragColor.g + 0.072 * FragColor.b;

    /// 转为灰度图
//    float grayscale = 0.2126 * inputColor.r + 0.7152 * inputColor.g + 0.0722 * inputColor.b;
//    FragColor.r = grayscale;
//    FragColor.g = grayscale;
//    FragColor.b = grayscale;
    FragColor.a = L;
}
