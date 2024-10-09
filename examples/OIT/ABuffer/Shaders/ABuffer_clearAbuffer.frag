#version 450 core
#extension GL_NV_gpu_shader5 : enable

layout(std140, binding = 0) uniform Matrix
{
    mat4 viewMat;
    mat4 projectMat;
    vec2 windowSize;
    vec4 backgroundColor;
    float alpha;
};

uniform layout(binding = 1, rgba32f) writeonly  image2DArray abuffers;
uniform layout(binding = 2, r32ui) writeonly uimage2D abufferCounter;

out vec4 FragColor;
uniform vec4 uColor;

void main()
{
    ivec3 coord = ivec3(gl_FragCoord.xy, 0);

    imageStore(abuffers, coord, vec4(0.0));
    imageStore(abufferCounter, coord.xy, uvec4(0));

    vec2 rg = gl_FragCoord.xy / windowSize;
    FragColor = vec4(rg,0.0,1.0);
}