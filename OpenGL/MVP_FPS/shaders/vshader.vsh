
#version 330 core

in vec3 vPosition;  
in vec3 vColor;    

out vec3 vColorOut; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(vPosition, 1.0); 
    vColorOut = vColor;
}
