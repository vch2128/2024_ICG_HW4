#version 330 core
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

uniform mat4 projection;
uniform float time;
float MAGNITUDE = 1.0f + sin(time) * 0.5f; 

void GenerateLine(int index){
    gl_Position = projection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = projection * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main() {
    for(int i = 0; i < 3; i++){
        GenerateLine(i);
    }
}