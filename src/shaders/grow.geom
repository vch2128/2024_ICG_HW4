#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 vFragPos[];
in vec3 vNormal[]; 
in vec2 vTexCoord[];

out vec3 gFragPos;
out vec3 gNormal;
out vec2 gTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform float time;

float inflateAmount = sin(time*3.0) * 0.2f;

void main() {
    for (int i = 0; i < 3; ++i) {
        vec3 inflatedPosition = vFragPos[i] + vNormal[i] * inflateAmount;
        gFragPos = inflatedPosition;
        gNormal = vNormal[i];
        gTexCoord = vTexCoord[i]; 

        gl_Position = projection * view * vec4(inflatedPosition, 1.0);
        EmitVertex();
    }
    EndPrimitive();
}
