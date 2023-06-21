#version 330

layout(location = 0)in vec3 vert;
layout(location = 1)in vec2 uv;
layout(location = 2)in vec3 normal;


uniform vec4 lightSource;
uniform TBlock {
    mat4 transform;
    mat4 viewingMat;
    mat4 projectionMat;
} my;

out vec3 Normal;
out vec3 lightDir;
out vec2 texCord;

void main()
{
    lightDir = normalize(lightSource.xyz);
    Normal = normalize(mat3(transpose(inverse(my.viewingMat *my.transform))) * vec3(normal));
    gl_Position = my.projectionMat * my.viewingMat * my.transform * vec4(vert, 1.0);
    texCord = uv;
};