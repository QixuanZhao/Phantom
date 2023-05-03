#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D textureBuffer;
uniform float alpha = 0.7;
uniform float exposure = 1.0;
uniform bool toneMapping = true;
uniform bool monochrome = false;

void main()
{
  vec3 textureColour = texture(textureBuffer, TexCoords).rgb;
  if (monochrome) textureColour.gb = textureColour.rr;
  if (toneMapping) textureColour = vec3(1.0) - exp(-textureColour * exposure);
  
  FragColor = vec4(textureColour, alpha);
}