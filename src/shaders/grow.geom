#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

in DATA {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} gs_in;

out vec3 gFragPos;
out vec3 gNormal;
out vec2 gTexCoord;

uniform float time;
float inflateAmount = sin(time*3.0) * 0.2f;

void main() {
    for (int i = 0; i < 3; ++i) {
        vec3 inflatedPosition = gs_in.FragPos[i] + gs_in.Normal[i] * inflateAmount;
        gFragPos = inflatedPosition;
        gNormal = gs_in.Normal[i];
        gTexCoord = gs_in.TexCoord[i]; 
        gl_Position = gl_in[i].gl_Position + vec4(gs_in.Normal[i] * inflateAmount, 0.0);
        EmitVertex();
    }
    EndPrimitive();
}
