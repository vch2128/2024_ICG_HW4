#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

in vec3 vFragPosView[];
in vec3 vNormal[]; 
in vec3 vNormalView[]; 
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
        vec3 inflatedPosition = vFragPosView[i] + vNormalView[i] * inflateAmount;
        gFragPos = vec3(inverse(view) * vec4(inflatedPosition, 1.0));
        gNormal = vNormal[i];
        gTexCoord = vTexCoord[i]; 

        gl_Position = projection * vec4(inflatedPosition, 1.0);
        EmitVertex();
    }
    EndPrimitive();
}
