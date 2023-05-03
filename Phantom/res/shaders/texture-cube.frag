#version 460 core
out vec4 FragColor;
  
in vec3 TexCoords;

uniform samplerCube textureBuffer;
uniform float alpha = 0.7;

void main()
{
    FragColor = vec4(texture(textureBuffer, TexCoords).rgb, alpha);
}