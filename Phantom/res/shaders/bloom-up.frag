#version 460 core

out vec4 outputColour;

in vec2 TexCoords;

uniform sampler2D source;
uniform sampler2D target;
uniform float radius = 1.0;

uniform float mixFactor = 0.5;

void main () {
    vec3 targetColour = texture(target, TexCoords).rgb;

    vec2 sourceUnit = radius / textureSize(source, 0);

    vec3 a = texture(source, TexCoords + vec2(-1.0, -1.0) * sourceUnit).rgb;
    vec3 b = texture(source, TexCoords + vec2( 0.0, -1.0) * sourceUnit).rgb;
    vec3 c = texture(source, TexCoords + vec2( 1.0, -1.0) * sourceUnit).rgb;
    vec3 d = texture(source, TexCoords + vec2(-1.0,  0.0) * sourceUnit).rgb;
    vec3 e = texture(source, TexCoords + vec2( 0.0,  0.0) * sourceUnit).rgb;
    vec3 f = texture(source, TexCoords + vec2( 1.0,  0.0) * sourceUnit).rgb;
    vec3 g = texture(source, TexCoords + vec2(-1.0,  1.0) * sourceUnit).rgb;
    vec3 h = texture(source, TexCoords + vec2( 0.0,  1.0) * sourceUnit).rgb;
    vec3 i = texture(source, TexCoords + vec2( 1.0,  1.0) * sourceUnit).rgb;

    vec3 sourceColour = a + 2.0 * b + c;
    sourceColour += 2.0 * (d + 2.0 * e + f);
    sourceColour += g + 2.0 * h + i;
    sourceColour /= 16.0;

    outputColour = vec4(mix(targetColour, sourceColour, mixFactor), 1.0);
}