#version 460 core

out vec2 outputColour;

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
    vec2 AtoD[2][2];
    vec2 E = texture(source, TexCoords).rg;

    vec2 sampleColour = vec2(0.0);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            AtoD[i][j] = texture(source, TexCoords + (vec2(i, j) * 2.0 - 1.0) * unit).rg;
            sampleColour += AtoD[i][j];
        }
    }

    sampleColour /= 8.0;
    sampleColour += E / 2.0;

    outputColour = sampleColour;
}