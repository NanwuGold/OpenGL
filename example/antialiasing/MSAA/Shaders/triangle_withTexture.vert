#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 a_TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 v_TexCoord;

void main()
{
    vec4 pos = vec4(position, 1.0f);
    v_TexCoord = a_TexCoord;

    gl_Position = pos;
}