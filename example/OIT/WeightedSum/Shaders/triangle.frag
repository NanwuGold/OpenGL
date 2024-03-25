#version 450 core

layout(location = 0) out vec4 fragColor;

in vec4 v_color;

void main()
{
    fragColor = vec4(1.0,0.0,0.0,1.0);
    fragColor = v_color;
}




