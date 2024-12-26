#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;

void main()
{
    vec3 modPos = aPos;
    modPos.y += sin(time + aPos.x * 10.0) * 0.1;
    gl_Position = projection * view * model * vec4(modPos, 1.0);
    TexCoord = aTexCoord;
    FragPos = vec3(model * vec4(modPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
}