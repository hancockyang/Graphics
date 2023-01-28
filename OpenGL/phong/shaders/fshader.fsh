#version 330 core

in vec3 worldpos;  
in vec2 texcoord;  
in vec3 normal;

out vec4 fColor;

uniform sampler2D texture;

uniform vec3 lightpos;
uniform vec3 camerapos;

float phong(vec3 worldpos, vec3 camerapos, vec3 lightpos, vec3 normal)
{
    vec3 N = normalize(normal);                // normal
    vec3 V = normalize(worldpos - camerapos);  
    vec3 L = normalize(worldpos - lightpos);
    vec3 R = reflect(L, N);

    float ambient = 0.3;
    float diffuse = max(dot(N, -L), 0) * 0.7;
    float specular = pow(max(dot(-R, V), 0), 50.0) * 1.1;
 
    return ambient + diffuse + specular;
}

void main()
{
    fColor.rgb = texture2D(texture, texcoord).rgb;
    fColor.rgb *= phong(worldpos, camerapos, lightpos, normal);
}
