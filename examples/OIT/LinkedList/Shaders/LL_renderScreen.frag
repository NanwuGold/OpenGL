#version 450 core

#define MAXNODECOUNT 20

struct Node
{
    vec4 color;
    float depth;
    uint next;
};

layout(binding = 1, std430) buffer NodeBuffer
{
    Node nodes[];
};
layout(binding = 2, r32ui) uniform uimage2D headPointerImage;

uniform vec4 backgroundColor;
out vec4 vFragColor;

void main()
{
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

    /// 对获取的节点排序
    for(int i = 0; i < nodeCount - 1; i++)
    {
        for(int j = i + 1; j < nodeCount; j++)
        {
            if(nodeLists[i].depth > nodeLists[j].depth)
            {
                Node temp = nodeLists[i];
                nodeLists[i] = nodeLists[j];
                nodeLists[j] = temp;
            }
        }
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
        vec3 outColor = (srcColor.rgb * srcAlpha + color.rgb * dstAlpha * (1.0 - srcAlpha)) / outAlpha;
        color = vec4(outColor, outAlpha);
    }

    /// 最终的颜色
    vFragColor = color;
}

