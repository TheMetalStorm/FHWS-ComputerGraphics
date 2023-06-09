#version 330

in vec3 lightDir;
in vec3 Normal;
in vec2 texCord;
out vec4 fragColor;
uniform sampler2D tex;

vec4 toonify(float intensity) {
    vec4 color;
    if (intensity > 0.98) color = vec4(0.8, 0.8, 0.8, 1.0);
    else if (intensity > 0.5) color = vec4(0.4, 0.4, 0.8, 1.0);
    else if (intensity > 0.25) color = vec4(0.2, 0.2, 0.4, 1.0);
    else color = vec4(0.1, 0.1, 0.1, 1.0);
    return color;
}

void main() {
    vec3 n = normalize(Normal);
    float intensity = max(dot(lightDir, n), 0.0);
    fragColor = toonify(intensity);// * texture(tex, texCord);


}
