#version 330 core

// TODO 3:
// Implement Gouraud shading
out vec4 FragColor;

in vec2 TexCoord;
in vec4 ambient;
in vec4 diffuse;
in vec4 specular;

uniform sampler2D ourTexture;

void main()
{
    vec4 objColor = texture(ourTexture, TexCoord);
    FragColor = ambient * objColor + diffuse * objColor + specular;
}