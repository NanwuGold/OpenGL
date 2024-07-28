#version 450 core
out vec4 FragColor;

in vec4 vColor;
uniform vec4 uColor;

void main()
{
    FragColor = vColor;
}