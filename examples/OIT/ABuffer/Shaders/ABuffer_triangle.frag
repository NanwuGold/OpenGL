#version 450 core
out vec4 FragColor;

layout(std140, binding = 0) uniform Matrix
{
    mat4 viewMat;
    mat4 projectMat;
    vec2 windowSize;
    vec4 backgroundColor;
    float alpha;
};

layout(binding = 1, rgba32f) coherent uniform image2DArray abuffers;
layout(binding = 2, r32ui) coherent uniform uimage2D abufferCounter;

/**
在 GLSL 中，coherent 是一个修饰符，用于指示某些变量（如图像纹理或缓冲区）在并行执行的上下文中具有一致性和可见性。具体来说，它的作用包括：
主要功能
内存一致性：coherent 确保在多个线程（如计算着色器或片段着色器）并行访问同一变量时，所有线程都能看到最新的值。这对于避免数据竞争和不一致性非常重要。
原子操作：当与原子操作结合使用时，coherent 确保在执行原子操作时，其他线程能够看到这些操作的结果。这对于图像纹理和缓冲区的原子更新尤为重要。
**/

uniform vec4 uColor;

void main()
{
    ivec2 fragCoord = ivec2(gl_FragCoord.xy);
    uint index = imageAtomicAdd(abufferCounter, fragCoord, 1);
    vec4 abufferVal = vec4(uColor.rgb, gl_FragCoord.z);
    imageStore(abuffers, ivec3(fragCoord, index), abufferVal);
    FragColor = vec4(uColor.rgb, gl_FragCoord.z);
}