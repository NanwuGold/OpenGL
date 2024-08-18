#version 450 core
out vec4 FragColor;

// in vec4 vColor;
// uniform vec4 uColor;

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

layout(std430, binding = 1) buffer testSSBOBuffer
{
    Node nodes[];
};

layout(binding = 2, offset = 0) uniform atomic_uint atomicCounter;



void main()
{

    uint nodeIndex = atomicCounterIncrement(atomicCounter);
    vec2 fragCoord = gl_FragCoord.xy;

    Node newNode;
    newNode.color = vec4(1.0);
    newNode.depth = gl_FragCoord.z;

    /// 获取前一次的指向的位置的


    // FragColor = vColor;
    FragColor = color;
}