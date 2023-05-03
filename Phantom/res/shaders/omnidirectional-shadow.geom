#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform vec3 light_position;
uniform mat4 pv[6];

out vec4 FragPos; // FragPos from GS (output per emitvertex)

int getOctant(vec3 direction) {
    float x = direction.x;
    float y = direction.y;
    float z = direction.z;
    
    bool positiveX = (x > 0);
    bool positiveY = (y > 0);
    bool positiveZ = (z > 0);

    if (positiveZ) {
        if (positiveY) {
            if (positiveX) return 1;
            else return 2;
        } else if (positiveX) return 4;
        else return 3;
    } else if (positiveY) {
        if (positiveX) return 5;
        else return 6;
    } else if (positiveX) return 8;
    else return 7;
}

int getLayer(vec3 direction) {
    float x = direction.x;
    float y = direction.y;
    float z = direction.z;

    bool positiveX = (x > 0);
    bool positiveY = (y > 0);
    bool positiveZ = (z > 0);

    float xlength = abs(x);
    float ylength = abs(y);
    float zlength = abs(z);

    if (xlength > ylength && xlength > zlength) {
        return positiveX ? 0 : 1;
    } else if (ylength > zlength) {
        return positiveY ? 2 : 3;
    } else {
        return positiveZ ? 4 : 5;
    }
}

float getMaxAngle(vec3 dir1, vec3 dir2, vec3 dir3) {
    vec3 d1 = normalize(dir1);
    vec3 d2 = normalize(dir2);
    vec3 d3 = normalize(dir3);

    float c1 = dot(d1, d2);
    float c2 = dot(d2, d3);
    float c3 = dot(d3, d1);

    float minCosine = min(c1, min(c2, c3));
    return degrees(acos(minCosine));
}

void main()
{
    
    vec3 dir[3];
    int layers[3];

    bool layersPresent[6] = bool[] (
        false, // +x
        false, // -x
        false, // +y
        false, // -y
        false, // +z
        false  // -z
    );
    int presentedLayerCount = 0;

    for (int i = 0; i < 3; i++) {
        dir[i] = gl_in[i].gl_Position.xyz - light_position;
        layers[i] = getLayer(dir[i]);
        if (!layersPresent[layers[i]]) {
            presentedLayerCount++;
            layersPresent[layers[i]] = true;
        }
    }
    
    if (presentedLayerCount > 1) {
        for (int i = 0; i < 6; ++i)
            layersPresent[i] = true;
    }

    for (int i = 0; i < 6; i++) {
        // if (layersPresent[i]) {
            vec4 pos[3];

            pos[0] = pv[i] * gl_in[0].gl_Position;
            pos[1] = pv[i] * gl_in[1].gl_Position;
            pos[2] = pv[i] * gl_in[2].gl_Position;
            
            // vec4 ab = pos[1] - pos[0];
            // vec4 bc = pos[2] - pos[1];
            // float z = cross(vec3(ab.xy, 0.0), vec3(bc.xy, 0.0)).z;

            gl_Layer = i;
            // if (z >= 0) gl_Layer += 6; // back face

            for (int j = 0; j < 3; ++j) {
                FragPos = gl_in[j].gl_Position;
                gl_Position = pos[j];
                EmitVertex();
            }
            EndPrimitive();
        // }
    }
}  