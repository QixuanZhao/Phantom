#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vec4 fragmentNormal;
out vec4 fragmentNormalView;
out vec4 fragmentPosition;
out vec4 fragmentPositionView;
out vec2 textureCoordinate;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 lightPV;

void main() {
	fragmentNormal = model * vec4(aNormal, 0.0);
    fragmentNormalView = view * fragmentNormal;
	fragmentPosition = model * vec4(aPos, 1.0);
    fragmentPositionView = view * fragmentPosition;
	textureCoordinate = aTex;

	// gl_Position = lightPV * fragmentPosition;
	gl_Position = projection * fragmentPositionView;
	gl_PointSize = clamp(500.0 / gl_Position.z, 1.0, 6.2);
}