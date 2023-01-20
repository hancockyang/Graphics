
#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexcoord;
layout (location = 2) in vec3 vNormal;

out vec2 texcoord; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(vPosition, 1.0); 
    texcoord = vTexcoord;
}
