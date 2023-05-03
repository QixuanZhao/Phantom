#version 460 core

out float outputColour;

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

    float a = texture(source, TexCoords + vec2(-1.0, -1.0) * sourceUnit).r;
    float b = texture(source, TexCoords + vec2( 1.0, -1.0) * sourceUnit).r;
    float c = texture(source, TexCoords + vec2(-1.0,  1.0) * sourceUnit).r;
    float d = texture(source, TexCoords + vec2( 1.0,  1.0) * sourceUnit).r;

    float e = texture(source, TexCoords + vec2(-2.0,  0.0) * sourceUnit).r;
    float f = texture(source, TexCoords + vec2( 2.0,  0.0) * sourceUnit).r;
    float g = texture(source, TexCoords + vec2( 0.0, -2.0) * sourceUnit).r;
    float h = texture(source, TexCoords + vec2( 0.0,  2.0) * sourceUnit).r;

    float sourceColour = (a + b + c + d) / 12.0;
    sourceColour += (e + f + g + h) / 6.0;

    outputColour = sourceColour;
}