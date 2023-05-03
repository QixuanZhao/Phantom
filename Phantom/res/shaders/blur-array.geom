#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform bool horizontal;

in vec2 vTexCoords[];
out vec2 TexCoords;

void main () {
    gl_Layer = horizontal ? 1 : 0;

    for (int i = 0; i < 3; i++) {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    
    EndPrimitive();
}