#version  450 core

layout(location = 0) out vec4 fragColor;

in vec4 vColor;

uniform vec4 oColor;
uniform sampler2D depthTex;
uniform vec2 wSize;

void main()
{
    float depthVal = texture(depthTex,gl_FragCoord.xy / wSize).r;

    if(gl_FragCoord.z <= depthVal)
    {
        discard;
    }

    fragColor = oColor; /// show color
}
