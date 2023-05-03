#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;

out vec3 originalColour;
out vec3 fragmentPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	vec4 fragmentPos = model * vec4(aPos, 1.0f);
	gl_Position = projection * view * fragmentPos;
	originalColour = aColour;
	fragmentPosition = fragmentPos.xyz;
}