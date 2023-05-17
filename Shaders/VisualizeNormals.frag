#version 330

in vec3 Normal;
out vec4 fragColor;

void main()
{
    fragColor = vec4(0.5 + 0.5 * Normal, 1.0);
}