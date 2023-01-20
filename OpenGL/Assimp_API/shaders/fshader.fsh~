#version 330 core

in vec3 vColorOut;  
in vec2 texcoord;  

out vec4 fColor;

uniform sampler2D Texture;

void main()
{
    fColor.rgb = texture2D(Texture, vec2(texcoord.s, 1.0  - texcoord.t)).rgb;
}
