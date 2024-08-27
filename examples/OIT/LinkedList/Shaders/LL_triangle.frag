#version 450 core

layout(std140, binding = 0) uniform Matrix
{
    mat4 viewMat;
    vec4 color;
    mat4 projectMat;
};

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
layout(binding = 3, offset = 0) uniform atomic_uint atomicCounter;

out vec4 FragColor;

uniform vec4 uColor;

void main()
{
    vec2 fragCoord = gl_FragCoord.xy;
    uint nodeIndex = atomicCounterIncrement(atomicCounter);  ///< 当前片段在Nodebuffer的位置

    /// 获取前一次的指向的位置的
    uint prevHead = imageAtomicExchange(headPointerImage, ivec2(fragCoord), nodeIndex);

    nodes[nodeIndex].color = uColor;   /// 片段颜色
    nodes[nodeIndex].depth = gl_FragCoord.z;  /// 片段深度
    nodes[nodeIndex].next = prevHead;

    FragColor = uColor;
}