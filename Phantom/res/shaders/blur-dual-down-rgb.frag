#version 460 core

out vec3 outputColour;

in vec2 TexCoords;

uniform sampler2D source;

void main () {
    ivec2 size = textureSize(source, 0);
    vec2 unit = 1.0 / size;

    /**
    * A   B
    *   E  
    * C   D
    */
    vec3 AtoD[2][2];
    vec3 E = texture(source, TexCoords).rgb;

    vec3 sampleColour = vec3(0.0);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            AtoD[i][j] = texture(source, TexCoords + (vec2(i, j) * 2.0 - 1.0) * unit).rgb;
            sampleColour += AtoD[i][j];
        }
    }

    sampleColour /= 8.0;
    sampleColour += E / 2.0;

    outputColour = sampleColour;
}