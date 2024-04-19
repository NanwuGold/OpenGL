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

    sumOfAlpha = max(sumOfAlpha,1e-5);
    vec3 outColor1 = vec3((rgba.rgb / rgba.a) * (1.0f - sumOfAlpha));
    vec3 out2 = backGroundColor.rgb * sumOfAlpha;
    fragColor = vec4(outColor1 + out2,1.0f);
}


