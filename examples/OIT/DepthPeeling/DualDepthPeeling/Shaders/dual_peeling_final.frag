#version 330 core

out vec4 FragColor;
in vec2 vTexCoord;

uniform sampler2D FrontBlenderTex;  /// 被合并的前外层
uniform sampler2D BackBlenderTex;   /// 被合并的后外层
uniform sampler2D DepthBlenderTex;

void main()
{
    vec3 backRes = texture(BackBlenderTex,vTexCoord).rgb;
    vec4 frontRes = texture(FrontBlenderTex,vTexCoord);
    FragColor.rgb = frontRes.rgb + backRes * (1.0 - frontRes.a);
    FragColor.a = 1.0;
}
