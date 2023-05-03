#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vec3 fragmentNormal;
out vec3 fragmentPosition;
out vec2 textureCoordinate;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	fragmentNormal = aNormal;
	fragmentPosition = vec3(model * vec4(aPos, 1.0));
	textureCoordinate = aTex;

	gl_Position = projection * view * vec4(fragmentPosition, 1.0);
	gl_PointSize =  clamp(500.0 / gl_Position.z, 1.0, 6.2);
}