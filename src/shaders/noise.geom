#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 TexCoord;
    vec3 normal;
} gs_in[];

out vec2 TexCoords;

uniform float time;

vec3 GetNormal(){
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
} 

float rand(vec3 seed){
    return fract(sin(dot(seed ,vec3(12.9898,78.233,45.164))) * 43758.5453);
}

vec4 sandify(vec4 position, float timeOffset){
    vec3 randomOffset = vec3(
        rand(position.xyz + timeOffset) - 0.5,
        rand(position.yzx + timeOffset) - 0.5,
        rand(position.zxy + timeOffset) - 0.5
    ) * 2.0;
    float gravity = -timeOffset * 0.5;
    return position + vec4(randomOffset.x, randomOffset.y + gravity, randomOffset.z, 0.0);
}

void main() {
    float timeOffset = (sin(time) + 1.0) / 2.0;
    for(int i = 0; i < 3; i++){
        gl_Position = sandify(gl_in[i].gl_Position, timeOffset);
        TexCoords = gs_in[i].TexCoord;
        EmitVertex();
    }
    EndPrimitive();
}