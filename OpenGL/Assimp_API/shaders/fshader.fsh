#version 330 core

in vec2 texcoord;  

out vec4 fColor;

uniform sampler2D texture;

void main()
{
    fColor.rgb = texture2D(texture, texcoord).rgb;
}
