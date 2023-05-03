#version 460 core
out float FragColor;
  
in vec2 TexCoords;

uniform bool useSource = false;
uniform sampler2D source;
uniform sampler2DArray image;
  
uniform bool horizontal;

// uniform float weight[samplingSize] = float[] (
//   0.117412037,
//   0.108316078,
//   0.092194092,
//   0.072421176,
//   0.052532264,
//   0.035228951,
//   0.021895402
// );
uniform float weight[] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    if (useSource) {
        float z = 1 - gl_Layer;
        vec2 tc = TexCoords;
        float currFragRed = texture(source, tc).r;

        vec2 tex_offset = 1.0 / textureSize(source, 0); // gets size of single texel
        float result = currFragRed * weight[0] * 2; // current fragment's contribution

        for(int i = 1; i < weight.length(); ++i)
        {
            if (horizontal) {
                float left = texture(source, tc + vec2(tex_offset.x * i, 0.0)).r;
                float right = texture(source, tc - vec2(tex_offset.x * i, 0.0)).r;
                result += left * weight[i];
                result += right * weight[i];
            } else {
                float up = texture(source, tc + vec2(0.0, tex_offset.y * i)).r;
                float down = texture(source, tc - vec2(0.0, tex_offset.y * i)).r;
                result += up * weight[i];
                result += down * weight[i];
            }
        }

        FragColor = result;
    } else {
        float z = 1 - gl_Layer;
        vec3 tc = vec3(TexCoords, z);
        float currFragRed = texture(image, tc).r;

        vec2 tex_offset = 1.0 / textureSize(image, 0).xy; // gets size of single texel
        float result = currFragRed * weight[0] * 2; // current fragment's contribution

        for(int i = 1; i < weight.length(); ++i)
        {
            if (horizontal) {
                float left = texture(image, tc + vec3(tex_offset.x * i, 0.0, 0.0)).r;
                float right = texture(image, tc - vec3(tex_offset.x * i, 0.0, 0.0)).r;
                result += left * weight[i];
                result += right * weight[i];
            } else {
                float up = texture(image, tc + vec3(0.0, tex_offset.y * i, 0.0)).r;
                float down = texture(image, tc - vec3(0.0, tex_offset.y * i, 0.0)).r;
                result += up * weight[i];
                result += down * weight[i];
            }
        }

        FragColor = result;
    }
}