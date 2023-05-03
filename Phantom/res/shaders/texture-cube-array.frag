#version 460 core
out vec4 FragColor;
  
in vec3 TexCoords;

uniform samplerCubeArray textureBuffer;
uniform float alpha = 0.7;
uniform float mixCoefficient = 0.5;

void main()
{
    vec3 colour0 = texture(textureBuffer, vec4(TexCoords, 0.0)).rgb;
    vec3 colour1 = texture(textureBuffer, vec4(TexCoords, 1.0)).rgb;
    // vec3 colour0 = texture(textureBuffer, TexCoords).rgb;
    // vec3 colour1 = texture(textureBuffer, TexCoords).rgb;
    FragColor = vec4(mix(colour0, colour1, mixCoefficient), alpha);
}