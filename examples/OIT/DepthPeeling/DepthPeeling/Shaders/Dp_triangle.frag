#version  450 core

layout(location = 0) out vec4 fragColor;

in vec4 vColor;
uniform vec4 oColor;

void main()
{
    fragColor = vec4(oColor);
}
