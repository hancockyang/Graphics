
#version 330 core

in vec3 vPosition;  
in vec2 vTexcoord;    

out vec2 texcoord; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(vPosition, 1.0); 
    texcoord = vTexcoord;
}
