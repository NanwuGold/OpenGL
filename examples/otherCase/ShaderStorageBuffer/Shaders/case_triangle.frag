#version 450 core
out vec4 FragColor;

in vec4 vColor;
uniform vec4 uColor;

layout(std430, binding = 1) buffer testSSBOBuffer
{
    float data[];
};

void main()
{
    // FragColor = vColor;
    FragColor = vec4(data[0], data[1], data[2],1.0);
}