#version 450 core

in vec2 v_TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture_PassCount;

out vec4 fragColor;

void main()
{
    vec4 backGroundColor = texture(texture1,v_TexCoord).rgba;
    vec4 rgba = texture(texture2,v_TexCoord).rgba;
    vec4 temp = texture(texture_PassCount,v_TexCoord);
    if(int(temp.r) == 0)
    {
        fragColor = backGroundColor;
        return ;
    }
    fragColor = vec4(vec3(backGroundColor*(1 - rgba.a) + rgba *  rgba.a),1.0);
}


