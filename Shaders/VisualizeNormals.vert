#version 330
                         
layout(location = 0)in vec3 vert;
layout(location = 1)in vec2 uv;
layout(location = 2)in vec3 normal;

out vec3 Normal;

uniform TBlock {
   mat4 transform;
   mat4 viewingMat;
   mat4 projectionMat;
} my;

void main()
{
    Normal = normal;
    gl_Position = my.projectionMat * my.viewingMat * my.transform * vec4(vert, 1.0);
};