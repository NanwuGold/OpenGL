#version 450 core

struct Node
{
    vec4 color;
    float depth;
    uint next;
};


layout(binding = 2, r32ui) uniform uimage2D headPointerImage;
layout(std430, binding = 1) buffer NodeBuffer
{
    Node nodes[];
};


out vec4 vFragColor;
in vec2 vTexCoord;


void main()
{
    vec2 frageCoord = gl_FragCoord.xy;
    uint head = imageLoad(headPointerImage, ivec2(frageCoord)).r;

    vec4 color = vec4(0.0);
    float alphaSum = 0.0;

    while(head != 0xffffffff)
    {
        Node node = nodes[head];
        color += node.color * (1.0 - alphaSum);
        alphaSum += node.color.a;
        head = node.next;
    }
    vFragColor = color;
}

