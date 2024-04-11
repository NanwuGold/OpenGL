#version 450 core

in vec2 v_TexCoord;

uniform sampler2D texture1;  ///< back ground color
uniform sampler2D texture2;  ///< sum of rgb
uniform sampler2D texture3;  ///< count of pixels
uniform sampler2D texture4;  ///< accuml alpha

out vec4 fragColor;

void main()
{
    vec4 backGroundColor = texture(texture1,v_TexCoord).rgba;
    vec4 rgba = texture(texture2,v_TexCoord).rgba;
    float count = texture(texture3,v_TexCoord).r;
    float sumOfAlpha = texture(texture4,v_TexCoord).r;

    vec3 outColor = (rgba.rgb / rgba.a) * (1.0 - sumOfAlpha) + backGroundColor.rgb * sumOfAlpha;
    fragColor = vec4(outColor,1.0);
}


