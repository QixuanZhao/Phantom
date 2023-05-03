#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform float exposure = 0.5;
uniform bool toneMappingEnabled = true;
uniform bool reinhard = false;
uniform float gamma = 2.2;

void main()
{
  vec3 outputColour = texture(hdrBuffer, TexCoords).rgb;

  vec3 mapped;
  if (!toneMappingEnabled) 
    mapped = outputColour;
  else if (reinhard)
    mapped = outputColour / (outputColour + vec3(1.0));
  else
    mapped = vec3(1.0) - exp(-outputColour * exposure);
  
  FragColor = vec4(pow(mapped, vec3(1.0 / gamma)), 1.0);
}