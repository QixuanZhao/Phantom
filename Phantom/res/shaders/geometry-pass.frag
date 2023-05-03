#version 460 core

layout (location = 0) out vec3 output_position;
layout (location = 1) out vec3 output_normal;
layout (location = 2) out vec4 output_albedo;
layout (location = 3) out vec3 output_position_view;
layout (location = 4) out vec3 output_normal_view;
layout (location = 5) out vec2 output_metallic_roughness;
layout (location = 6) out vec3 output_distinction_coefficient;

in vec4 fragmentNormal;
in vec4 fragmentNormalView;
in vec4 fragmentPosition;
in vec4 fragmentPositionView;
in vec2 textureCoordinate;

uniform float shininess = 32.0;

uniform float metallic = 0.5;
uniform float roughness = 0.5;
uniform vec3 K = vec3(0.0); // distinction coefficient

uniform vec3 originalColour = vec3(1.0);

void main() {
    output_position = fragmentPosition.xyz;
    output_normal = normalize(fragmentNormal.xyz);
    output_albedo = vec4(originalColour, shininess);
    output_position_view = fragmentPositionView.xyz;
    output_normal_view = normalize(fragmentNormalView.xyz);
    output_metallic_roughness = vec2(metallic, roughness);
    output_distinction_coefficient = K;
    gl_FragDepth = gl_FragCoord.z;
}