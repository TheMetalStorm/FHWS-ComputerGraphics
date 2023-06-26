#version 330

layout(location = 0)in vec3 vert;
layout(location = 1)in vec2 uv;
layout(location = 2)in vec3 normal;


uniform vec4 lightSource;
uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform TBlock {
    mat4 transform;
    mat4 viewingMat;
    mat4 projectionMat;
} my;


out vec2 texCord;
out vec4 vertex_color;



// https://github.com/ruange/Gouraud-Shading-and-Phong-Shading
//https://teaching.csse.uwa.edu.au/units/CITS3003/lectures/017_Shading_in_OpenGL.pdf
void main()
{
    vec3 n = normalize(mat3(transpose(inverse(my.viewingMat *my.transform))) * vec3(normal));
    vec4 pos4 = my.viewingMat * my.transform * vec4(vert, 1.0);
    vec3 pos = pos4.xyz;

    vec3 L = normalize(lightSource.xyz - pos);
    vec3 V = normalize(-pos);
    vec3 H = normalize(L+V);


    float intensity = max(dot(n, L), 0.0);

    //TODO: surface reflection coefficient should come from outside
    //TODO: .5 is ambient reflectance (local) and should come from outside
    float surfReflection = .3;
    float ambientReflectance = .2;

    vec4 ambient =  ambientReflectance * ambientColor;
    vec4 diffuse = surfReflection*diffuseColor * intensity;

    float specularReflectance = .4;
    float specularFalloff = 32;

    vec4 specular = specularColor * specularReflectance * pow(max(dot(n, H), 0.0),specularFalloff);

    vertex_color = ambient+diffuse + specular;
    gl_Position = my.projectionMat * my.viewingMat * my.transform * vec4(vert, 1.0);;

};