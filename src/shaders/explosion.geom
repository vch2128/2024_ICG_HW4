#version 330 core

layout (triangles) in;
//layout (triangle_strip, max_vertices = 3) out;
layout (line_strip, max_vertices = 6) out;

out vec2 TexCoord;
out vec4 blastColor;

in DATA
{
    vec3 Normal;
	vec2 texCoord;
    mat4 projection;
} data_in[];

uniform float explode;

vec4 getExplosionColor(float distance) {
    vec3 color = mix(vec3(1.0, 0.5, 0.1), vec3(0.1, 0.1, 0.1), distance / 30);

    return vec4(color, 1.0);
}

void main()
{
    vec3 vector0 = vec3(gl_in[0].gl_Position - gl_in[1].gl_Position);
    vec3 vector1 = vec3(gl_in[2].gl_Position - gl_in[1].gl_Position);
    vec4 surfaceNormal = vec4(normalize(cross(vector0, vector1)), 0.0f);

    float start = 0.0;

    for (int i = 0; i < 3; ++i)
    {
        if(explode > 30){
            start = explode - 20;
        }

        // start pos
        gl_Position = data_in[i].projection * (gl_in[i].gl_Position + start * surfaceNormal);
        TexCoord = data_in[i].texCoord;
        blastColor = getExplosionColor(start);
        EmitVertex();

        // explode pos
        gl_Position = data_in[i].projection * (gl_in[i].gl_Position + explode * surfaceNormal);
        TexCoord = data_in[i].texCoord;
        blastColor = getExplosionColor(explode);
        EmitVertex();

        EndPrimitive();
    }

    //EndPrimitive();
}

