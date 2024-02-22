#version 450 core
layout(location = 0) out vec4 FragColor;

in vec4 inputColor;

void main()
{
    FragColor = inputColor;

    /// 亮度计算
    // vec3 light = vec3(0.299, 0.587, 0.114);
//    float L = sqrt(dot(FragColor.rgb,light));

    /// 转为灰度图
    //    float grayscale = 0.2126 * inputColor.r + 0.7152 * inputColor.g + 0.0722 * inputColor.b;
    //    FragColor.r = grayscale;
    //    FragColor.g = grayscale;
    //    FragColor.b = grayscale;
    FragColor.a = 1.0f;
}
