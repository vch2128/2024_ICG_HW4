#version 330 core
out vec4 FragColor;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 gFragPos;
in vec3 gNormal;
in vec2 gTexCoord;

uniform sampler2D ourTexture;
uniform Light light;
uniform vec3 CameraPos;

void main()
{
    vec3 norm = normalize(gNormal);
    vec3 lightDir = normalize(light.position - gFragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(CameraPos - gFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 result = (1.0 + diff + spec) * lightColor;

    vec4 texColor = texture(ourTexture, gTexCoord);
    FragColor = vec4(texColor.rgb * result, texColor.a);
}
