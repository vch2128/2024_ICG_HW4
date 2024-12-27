#version 330 core
out vec4 FragColor;

in vec4 blastColor;

uniform sampler2D ourTexture;


void main()
{
    FragColor = blastColor;
} 