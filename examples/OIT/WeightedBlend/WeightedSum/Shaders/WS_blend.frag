#version 450 core

out vec4 FragColor;
in vec2 v_TexCoord;

uniform sampler2D texture1;  /// background color
uniform sampler2D texture2;  /// transparent obj accum

void main()
{
    vec4 backColor = texture(texture1,v_TexCoord).rgba;
    vec4 rgba = texture(texture2,v_TexCoord).rgba;

    vec3 outColor = rgba.rgb - backColor.rgb * rgba.a + backColor.rgb;

    FragColor = vec4(outColor,1.0);
}