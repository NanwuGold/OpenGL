#version 450 core
layout(location = 0) out vec4 vFragColor;

uniform sampler2D inTex1;
uniform sampler2D inTex2;

in vec2 vTexCoord;

vec4 blendColor(vec3 srcColor,vec3 dstColor,float srcAlpha,float dstAlpha)
{
    vec4 color;
    // color.rgb = dstColor * dstAlpha + (1-dstAlpha) * srcColor * srcAlpha;
    color.rgb = dstColor * dstAlpha + (1 - dstAlpha) * srcColor * srcAlpha;

    color.a = srcAlpha - srcAlpha * dstAlpha + dstAlpha;
    /// color.a = srcAlpha *( 1.0 - dstAlpha) + dstAlpha
    color.rgb = color.rgb / color.a;

    return color;
}


void main()
{
    vec4 color1 = texture(inTex1,vTexCoord).rgba;
    vec4 color2 = texture(inTex2,vTexCoord).rgba;
    vFragColor = blendColor(color1.rgb,color2.rgb,color1.a,color2.a);
}