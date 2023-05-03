#version 460 core

uniform bool spotlight = false;
uniform vec3 direction;
uniform float outerCutoff;
uniform vec3 lightColour;

in vec3 fragPos;

layout (location = 0) out vec4 outputColour;

void main() {
  if (!spotlight)
    outputColour = vec4(lightColour, 1.0);
  else {
    float cosineCutoff = cos(outerCutoff);
    float cosine = dot(direction, normalize(fragPos));
    if (cosine > cosineCutoff) outputColour = vec4(lightColour, 1.0);
    else outputColour = vec4(lightColour * 1e-3, 1.0);
  }
}