#version 330 core
out vec4 FragColor;

uniform float time;

void main()
{
    float r = abs(sin(time * 1.0));
    float g = abs(sin(time * 0.5 + 1.0));
    float b = abs(sin(time * 0.25 + 2.0));
    FragColor = vec4(r, g, b, 1.0f);
}
