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

layout(binding = 1, rgba32f) coherent uniform image2DArray abuffers;   ///< ABuffer
layout(binding = 2, r32ui) coherent uniform uimage2D abufferCounter;   ///< 计数器

out vec4 vFragColor;

vec4 fragementList[MaxFragments];  ///< 获取所有片元

void main()
{
    ivec2 coord = ivec2(gl_FragCoord.xy);
    int abufferNum = int(imageLoad(abufferCounter, coord).r);   ///< 当前像素位置的片段数量

    for(int i=0; i < abufferNum; i++)
    {
        fragementList[i] = imageLoad(abuffers,ivec3(coord, i));  ///< 获取当前层的颜色
    }

    /// TODO: sort fragmentList
    vec4 finalColor = vec4(0.0);
    float calcAlpha = 0.0;
    /// 对获取的节点排序
    for(int i = 0; i < abufferNum - 1; i++)
    {
        for(int j = i + 1; j < abufferNum; j++)
        {
            if(fragementList[i].z > fragementList[j].z)
            {
                vec4 temp = fragementList[i];
                fragementList[i] = fragementList[j];
                fragementList[j] = temp;
            }
        }

        finalColor = fragementList[0];
        calcAlpha = 0.5f;
    }


    finalColor = finalColor + backgroundColor * (1.0f - calcAlpha);

    /// just out closet layer
    vFragColor = vec4(finalColor);

}

void sortAndOutput()
{
    #if  0
    vec2 frageCoord = gl_FragCoord.xy;

    uint head = imageLoad(headPointerImage, ivec2(frageCoord)).r;

    /// 收集当前片段的列表的所有颜色以及深度
    /// 并计算最终的颜色
    Node nodeLists[MAXNODECOUNT];
    int nodeCount = 0;
    uint headTemp = head;
    while(headTemp != 0xffffffff && nodeCount < MAXNODECOUNT)
    {
        nodeLists[nodeCount] = nodes[headTemp];   /// 从SSBO中获取节点
        headTemp = nodeLists[nodeCount].next;     /// 获取下一个节点
        nodeCount++;                             /// 下次循环时，下一个节点的存储的索引
    }

    /// 计算最终着色
    vec4 color = backgroundColor;
    if(nodeCount == 0)
    {
        vFragColor = backgroundColor;
        return ;
    }

    /// front to back的方式合并颜色
    for(int i = 0; i < nodeCount; i++)
    {
        vec4 srcColor = nodeLists[i].color;
        float srcAlpha = srcColor.a;
        float dstAlpha = color.a;

        float outAlpha = dstAlpha + srcAlpha - dstAlpha * srcAlpha;
        vec3 outColor = (srcColor.rgb * srcAlpha + color.rgb * dstAlpha * (1.0 - srcAlpha));
        color = vec4(outColor, outAlpha);
    }

    /// 最终的颜色
    vFragColor = color;
    #endif
}


