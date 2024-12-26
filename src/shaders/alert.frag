#version 330 core
out vec4 FragColor;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoord;
in vec3 Normal; 
in vec3 FragPos;

uniform sampler2D ourTexture;
uniform Light light;
uniform vec3 CameraPos;
uniform float time;

vec3 baseColor1 = vec3(1.0f, 0.5f, 0.0f); // orange
vec3 baseColor2 = vec3(1.0f, 0.2f, 0.0f); // red
vec3 baseColor3 = vec3(1.0f, 1.0f, 0.0f); // yellow


float noise(vec2 uv) {
    return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    // ambient + diffuse + specular 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    
    vec3 viewDir = normalize(CameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 result = (0.3 + diff + spec) * lightColor;
    vec4 texColor = texture(ourTexture, TexCoord);
    texColor = vec4(texColor.rgb * result, texColor.a);
    
    float intensity = abs(sin(time * 3.0));
    float randomNoise = noise(TexCoord + time * 0.1);

    vec3 fireColor;
    if (intensity < 0.33) {
        fireColor = mix(baseColor1, baseColor2, intensity * 3.0);
    } else if (intensity < 0.66) {
        fireColor = mix(baseColor2, baseColor3, (intensity - 0.33) * 3.0);
    } else {
        fireColor = mix(baseColor3, baseColor1, (intensity - 0.66) * 3.0);
    }

    fireColor += randomNoise * 0.1;

    vec3 finalColor = texColor.rgb * fireColor; 
    FragColor = vec4(finalColor, texColor.a);
}
