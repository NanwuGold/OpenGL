#version  450 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec4 vColor;

void main()
{
    gl_Position = projection * view * model *  vec4(aPosition,1.0) ;
    vColor = aColor;
}
