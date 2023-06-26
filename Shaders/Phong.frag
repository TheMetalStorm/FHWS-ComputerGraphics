#version 330


uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
in vec2 texCord;
in vec3 fN;
in vec3 fV;
in vec3 fL;


uniform sampler2D tex;

void main() {
    vec3 L = normalize(fL);
    vec3 V = normalize(fV);
    vec3 N = normalize(fN);

    vec3 H = normalize(L+V);


    float intensity = max(dot(N, L), 0.0);

    //TODO: surface reflection coefficient should come from outside
    //TODO: .5 is ambient reflectance (local) and should come from outside
    float surfReflection = .3;
    float ambientReflectance = .2;

    vec4 ambient =  ambientReflectance * ambientColor;
    vec4 diffuse = surfReflection*diffuseColor * intensity;

    float specularReflectance = .4;
    float specularFalloff = 32;

    vec4 specular = specularColor * specularReflectance * pow(max(dot(N, H), 0.0),specularFalloff);

    vec4 vertex_color = ambient+diffuse + specular;
    gl_FragColor = vertex_color;
    gl_FragColor.a = 1.0;

}
