#version 330

layout(location = 0)in vec3 vert;
layout(location = 1)in vec2 uv;
layout(location = 2)in vec3 normal;



uniform TBlock {
    mat4 transform;
    mat4 viewingMat;
    mat4 projectionMat;
} my;
uniform vec4 lightSource;


out vec2 texCord;
out vec3 fN;
out vec3 fV;
out vec3 fL;


// https://github.com/ruange/Gouraud-Shading-and-Phong-Shading
//https://teaching.csse.uwa.edu.au/units/CITS3003/lectures/017_Shading_in_OpenGL.pdf
void main()
{
    fN = mat3(transpose(inverse(my.viewingMat *my.transform))) * vec3(normal);
    vec4 pos4 = my.viewingMat * my.transform * vec4(vert, 1.0);
    fV = -pos4.xyz;
    fL = lightSource.xyz - vec3(pos4);

    gl_Position = my.projectionMat * my.viewingMat * my.transform * vec4(vert, 1.0);;

};