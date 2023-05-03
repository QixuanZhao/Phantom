#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 fragmentPosition;

void main() {
    fragmentPosition = model * vec4(aPos, 1.0);
	gl_Position = projection * view * fragmentPosition;
	gl_PointSize =  clamp(500.0 / gl_Position.z, 1.0, 6.2);
}