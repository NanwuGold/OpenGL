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


layout(std430, binding = 1) buffer NodeBuffer
{
    Node nodes[];
};

layout(binding = 2, r32ui) uniform uimage2D headPointerImage;

layout(binding = 3, offset = 0) uniform atomic_uint atomicCounter;


out vec4 FragColor;

// in vec4 vColor;
uniform vec4 uColor;

void main()
{
    uint nodeIndex = atomicCounterIncrement(atomicCounter);  ///< 当前片段在Nodebuffer的位置
    vec2 fragCoord = gl_FragCoord.xy;

    Node newNode;
    newNode.color = uColor;   /// 片段颜色
    newNode.depth = gl_FragCoord.z;  /// 片段深度

    /// 获取前一次的指向的位置的

    uint prevHead = imageAtomicExchange(headPointerImage, ivec2(fragCoord), nodeIndex);
    newNode.next = prevHead;

    // 安全地写入新节点
    memoryBarrier();

    nodes[nodeIndex] = newNode;

    /// debug output
    // FragColor = vColor;
    // FragColor = color;
    // float al = float(nodeIndex) / 255.0f;
    // FragColor = vec4(uColor.rg, al, 1.0);

    FragColor = uColor;
}