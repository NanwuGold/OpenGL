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
    vec4 count = texture(texture_PassCount,v_TexCoord);

    float n = max(1.0,count.r);
    vec4 o = vec4(rgba.rgb / max(rgba.a,0.0001),pow(max(0.0,1.0 - rgba.a / n),n));
    if(o.a < 0.0001)
    {
        o.a = 1.0;
    }

    fragColor = vec4(vec3(backGroundColor * o.a + o * (1.0 - o.a)).rgb,1.0);


    // fragColor = vec4(backGroundColor.rgb * count.g + vec3(rgba.rgb / max(rgba.a,0.0001)) * (1.0 - count.g),1.0);


}


