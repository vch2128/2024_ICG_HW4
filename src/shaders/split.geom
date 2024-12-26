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
   return normalize(cross(b, a));
} 

vec4 explode(vec4 position, vec3 normal, float offset){
    vec3 direction = normal * offset;
    return position + vec4(direction, 0.0);
}

void main() {
    vec3 normal = -GetNormal();
    float offsetMagnitude = ((sin(time) + 1.0) / 2.0) * 2.0; 
    for(int i = 0; i < 3; i++){
        gl_Position = explode(gl_in[i].gl_Position, normal, offsetMagnitude);
        TexCoords = gs_in[i].TexCoord;
        EmitVertex();
    }
    EndPrimitive();

    for(int i = 0; i < 3; i++){
        gl_Position = explode(gl_in[i].gl_Position, -normal, offsetMagnitude);
        TexCoords = gs_in[i].TexCoord;
        EmitVertex();
    }
    EndPrimitive();
}
