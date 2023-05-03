#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool spotlight = false;
uniform vec3 direction;
uniform float outerCutoff;

out vec3 fragPos;

void main() {
	fragPos = normalize(aPos); 
	if (spotlight) {
		float cosineCutoff = cos(outerCutoff);
		float cosine = dot(direction, normalize(fragPos));
		if (cosine > cosineCutoff) {
			fragPos -= direction * (cosine - cosineCutoff);
		}
	}
	gl_Position = projection * view * model * vec4(fragPos, 1.0);
}