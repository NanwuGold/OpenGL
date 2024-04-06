#version 450 core

in vec2 v_TexCoord;

uniform sampler2D texture1;

out vec4 fragColor;

void main()
{
   vec4 rgba = texture(texture1,v_TexCoord).rgba;
   fragColor = rgba;
}


