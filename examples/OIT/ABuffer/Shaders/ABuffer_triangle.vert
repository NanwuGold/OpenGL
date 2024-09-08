#version 450 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;

layout(std140, binding = 0) uniform Matrix
{
    mat4 viewMat;
    mat4 projectMat;
    vec2 windowSize;
    vec4 backgroundColor;
    float alpha;
};


void main()
{
    gl_Position = projectMat * viewMat * model * vec4(aPos, 1.0);
}



