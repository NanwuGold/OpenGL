#version 450 core

#define MaxFragments 20   ///  最大层数

layout(std140, binding = 0) uniform Matrix
{
    mat4 viewMat;
    mat4 projectMat;
    vec2 windowSize;
    vec4 backgroundColor;
    float alpha;      ///< 全局的透明度
};

layout(binding = 1, r32ui) readonly uniform uimage2D abufferCounter;   ///< 计数器
layout(binding = 2, rgba32f) readonly uniform image2DArray abuffers;   ///< ABuffer

out vec4 vFragColor;

vec4 fragementList[MaxFragments];  ///< 获取所有片元

void main()
{
    ivec2 coord = ivec2(gl_FragCoord.xy);
    int abufferNum = int(imageLoad(abufferCounter, coord).r);   ///< 当前像素位置的片段数量

    if(abufferNum == 0)
    {
        vFragColor = backgroundColor;
        return ;
    }

    for(int i = 0; i < abufferNum; i++)
    {
        fragementList[i] = imageLoad(abuffers,ivec3(coord, i));  ///< 获取当前层的颜色和深度
    }

    /// 对获取的节点排序
    for(int i = 0; i < abufferNum - 1; i++)
    {
        for(int j = i + 1; j < abufferNum; j++)
        {
            if(fragementList[i].y > fragementList[j].y)
            {
                vec4 temp = fragementList[i];
                fragementList[i] = fragementList[j];
                fragementList[j] = temp;
            }
        }
    }

    vec4 color = backgroundColor;

    vec4 outColor = fragementList[0];
    float colorFloat = outColor.r;
    color = unpackUnorm4x8(floatBitsToUint(colorFloat));

    for(int i = 1; i < abufferNum; i++)
    {
        vec4 outColor = fragementList[i];
        float colorFloat = outColor.r;

        vec4 srcColor = color;
        float srcAlpha = color.a;

        vec4 dstColor = unpackUnorm4x8(floatBitsToUint(colorFloat));
        float dstAlpha = srcColor.a;

        float outAlpha = dstAlpha + srcAlpha - dstAlpha * srcAlpha;
        vec3 resColor = (srcColor.rgb * srcAlpha + dstColor.rgb * dstAlpha * (1.0 - srcAlpha));

        color = vec4(resColor, outAlpha);
    }

    /// 合并背景色与剥离的层
    vec4 srcColor = color;
    float srcAlpha = color.a;

    vec4 dstColor = backgroundColor;
    float dstAlpha = dstColor.a;

    float outAlpha = dstAlpha + srcAlpha - dstAlpha * srcAlpha;
    vec3 resColor = (srcColor.rgb * srcAlpha + dstColor.rgb * dstAlpha * (1.0 - srcAlpha));

    color = vec4(resColor, outAlpha);
    vFragColor = color;
}
