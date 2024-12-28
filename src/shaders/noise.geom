#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 TexCoord;
    vec3 normal;
} gs_in[];

out vec2 TexCoords;

uniform float time;
uniform mat4 view;
uniform mat4 projection;

float rand(vec3 seed){
    // random float value between 1.0 and 0.0
    return fract(sin(dot(seed ,vec3(12.9898,78.233,45.164))) * 43758.5453);
}

vec4 noise(vec4 position){
    vec3 randomOffset = vec3(
        rand(position.xyz) - 0.5,
        rand(position.yzx) - 0.5,
        rand(position.zxy) - 0.5
    ) * 2.0;
    return position + vec4(randomOffset.x, randomOffset.y, randomOffset.z, 0.0);
}

void main() {
    for(int i = 0; i < 3; i++){
        gl_Position = projection * view * noise(gl_in[i].gl_Position);
        TexCoords = gs_in[i].TexCoord;
        EmitVertex();
    }
    EndPrimitive();
}