#version 330 core

in vec3 vColorOut;  
out vec4 fColor;

void main()
{
    fColor = vec4(vColorOut, 1.0);
}
