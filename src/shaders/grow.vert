#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 vFragPosView;
out vec3 vNormal;
out vec3 vNormalView;
out vec2 vTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vFragPosView = vec3(view * model * vec4(aPos, 1.0)); 
    vNormal = mat3(transpose(inverse(model))) * aNormal;
    vNormalView = mat3(transpose(inverse(view * model))) * aNormal;
    vTexCoord = aTexCoord;
}
