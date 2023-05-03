#version 460 core

out vec3 outputColour;

in vec2 TexCoords;

uniform sampler2D source;
uniform float radius = 1.0;

void main () {
    vec2 sourceUnit = radius / textureSize(source, 0);
    
    /**
     *     G
     *   A   B
     * E       F
     *   C   D
     *     H
     */

    vec3 a = texture(source, TexCoords + vec2(-1.0, -1.0) * sourceUnit).rgb;
    vec3 b = texture(source, TexCoords + vec2( 1.0, -1.0) * sourceUnit).rgb;
    vec3 c = texture(source, TexCoords + vec2(-1.0,  1.0) * sourceUnit).rgb;
    vec3 d = texture(source, TexCoords + vec2( 1.0,  1.0) * sourceUnit).rgb;

    vec3 e = texture(source, TexCoords + vec2(-2.0,  0.0) * sourceUnit).rgb;
    vec3 f = texture(source, TexCoords + vec2( 2.0,  0.0) * sourceUnit).rgb;
    vec3 g = texture(source, TexCoords + vec2( 0.0, -2.0) * sourceUnit).rgb;
    vec3 h = texture(source, TexCoords + vec2( 0.0,  2.0) * sourceUnit).rgb;

    vec3 sourceColour = (a + b + c + d) / 12.0;
    sourceColour += (e + f + g + h) / 6.0;

    outputColour = sourceColour;
}