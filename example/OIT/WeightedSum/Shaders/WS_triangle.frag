#version 450 core

layout(location = 0) out vec4 fragColor;

in vec4 v_color;
uniform vec4 i_showColor;

void main()
{
    fragColor = vec4(i_showColor.rgb * i_showColor.a,i_showColor.a);
}




