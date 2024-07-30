#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 project;

/// OpenGL4.2 版本起 可以显示写入着色器的绑定点
layout(std140, binding = 0) uniform Matrix
{
    mat4 viewMat;
    vec4 color;
    mat4 projectMat;
};


out vec4 vColor;

void main()
{
    gl_Position = projectMat * viewMat * model * vec4(aPos, 1.0);
    // gl_Position = project * view * model * vec4(aPos, 1.0);
    // gl_Position = model * vec4(aPos, 1.0);
    vColor = aColor;
    vColor = color;
}



