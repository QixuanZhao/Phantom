#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

void main () {
    vec2 a = gl_in[0].gl_Position.xy;
    vec2 b = gl_in[1].gl_Position.xy;
    vec2 c = gl_in[2].gl_Position.xy;
    
    vec2 ab = b - a;
    vec2 bc = c - b;
    float z = cross(vec3(ab, 0.0), vec3(bc, 0.0)).z;

    if (z > 0) gl_Layer = 1;
    else gl_Layer = 0;

    for (int i = 0; i < 3; i++) {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    
    EndPrimitive();
}