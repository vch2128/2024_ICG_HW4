#version 330 core

// TODO 3:
// Implement Gouraud shading

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float gloss;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform Material material;
uniform Light light;
uniform vec3 CameraPos;

out vec2 TexCoord;
out vec4 ambient;
out vec4 diffuse;
out vec4 specular;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    vec3 WorldPos = vec3(model * vec4(aPos, 1.0));
    vec3 Normal = mat3(transpose(inverse(model))) * aNormal;

    // ambient
    vec4 La = vec4(light.ambient, 1.0f);
    vec4 Ka = vec4(material.ambient, 1.0f);
    ambient = La * Ka;

    // diffuse
    vec4 Ld = vec4(light.diffuse, 1.0f);
    vec4 Kd = vec4(material.diffuse, 1.0f);
    vec3 N = normalize(Normal);
    vec3 L = normalize(light.position - WorldPos);
    float diff = max(dot(L, N), 0.0);
    diffuse = Ld * Kd * diff;

    // specular
    vec4 Ls = vec4(light.specular, 1.0f);
    vec4 Ks = vec4(material.specular, 1.0f);
    vec3 V = normalize(CameraPos - WorldPos);
    vec3 R = normalize(reflect(-L, N));
    float spec = pow(max(dot(V, R), 0.0), material.gloss);
    specular = Ls * Ks * spec;
}