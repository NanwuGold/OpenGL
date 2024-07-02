#version 450 core

in vec2 v_TexCoord;

/// merge result 

uniform sampler2D texture1;  ///< back ground color
uniform sampler2D texture2;  ///< sum of rgb
uniform sampler2D texture3;  ///< sum Of Alpha (累积)

out vec4 fragColor;

void main()
{
    vec4 backGroundColor = texture(texture1,v_TexCoord).rgba;
    vec4 rgba = texture(texture2,v_TexCoord).rgba;
    float sumOfAlpha = texture(texture3,v_TexCoord).r;

    vec3 outColor = vec3((rgba.rgb / max(rgba.a,1e-5)) * (1.0f - sumOfAlpha)) + backGroundColor.rgb * sumOfAlpha;
    fragColor = vec4(outColor,1.0f);
}


