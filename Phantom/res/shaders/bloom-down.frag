#version 460 core

out vec4 outputColour;

in vec2 TexCoords;

uniform sampler2D source;

void main () {
  ivec2 size = textureSize(source, 0);
  vec2 unit = 1.0 / size;

  /**
   * A   B   C
   *   J   K  
   * D   E   F
   *   L   M  
   * G   H   I
   */
  vec3 AtoI[3][3];
  vec3 JtoM[2][2];

  vec3 wideSample = vec3(0.0);
  vec3 narrowSample = vec3(0.0);

  const float kernel[3][3] = {
    { 1.0, 2.0, 1.0 },
    { 2.0, 4.0, 2.0 },
    { 1.0, 2.0, 1.0 }
  };

  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      AtoI[i+1][j+1] = texture(source, TexCoords + (2.0 * vec2(i, j)) * unit).rgb;
      wideSample += kernel[i+1][j+1] * AtoI[i+1][j+1] / 32.0;
    }
  }

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      JtoM[i][j] = texture(source, TexCoords + (vec2(i, j) * 2.0 - 1.0) * unit).rgb;
      narrowSample += JtoM[i][j] / 4.0;
    }
  }

  outputColour = vec4(mix(wideSample, narrowSample, 0.5), 1.0);
}