#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightPV;

out vec4 lightSpacePosition;

void main() {
	vec4 fragmentPosition = model * vec4(aPos, 1.0);
	lightSpacePosition = lightPV * fragmentPosition;
	gl_Position = projection * view * fragmentPosition;
	gl_PointSize =  clamp(500.0 / gl_Position.z, 1.0, 6.2);
}