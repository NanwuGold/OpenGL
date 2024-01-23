#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 vTexCoord;

void main()
{
    vec4 pos = vec4(position, 1.0f);
    gl_Position = pos;
    vTexCoord = texCoord;
}