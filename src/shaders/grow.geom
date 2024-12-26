#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

in vec3 vFragPos[];
in vec3 vNormal[]; 
in vec2 vTexCoord[];

out vec3 gFragPos;
out vec3 gNormal;
out vec2 gTexCoord;

uniform float time;
float inflateAmount = sin(time*3.0) * 0.2f;

void main() {
    for (int i = 0; i < 3; ++i) {
        vec3 inflatedPosition = vFragPos[i] + vNormal[i] * inflateAmount;
        gFragPos = inflatedPosition;
        gNormal = vNormal[i];
        gTexCoord = vTexCoord[i]; 
        gl_Position = gl_in[i].gl_Position + vec4(vNormal[i] * inflateAmount, 0.0);
        EmitVertex();
    }
    EndPrimitive();
}
