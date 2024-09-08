#version 450 core

layout(std140, binding = 0) uniform Matrix
{
    mat4 viewMat;
    mat4 projectMat;
    vec2 windowSize;
    vec4 backgroundColor;
    float alpha;
};

layout(binding = 1, rgba32f) uniform image2DArray abuffers;
layout(binding = 2, r32ui) uniform uimage2D abufferCounter;

out vec4 FragColor;
uniform vec4 uColor;

void main()
{
    ivec3 coord = ivec3(gl_FragCoord.xy,1.0);

    imageStore(abuffers, coord, vec4(1.0,0.0,0.0,0.0));
    imageStore(abufferCounter, coord.xy, uvec4(1,0,0,1));

    vec2 rg = gl_FragCoord.xy / windowSize;
    FragColor = vec4(rg,0.0,1.0);
}