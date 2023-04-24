#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in  vec3 position;
layout (location = 1) in  vec3 normal;
layout (location = 2) in uvec4 boneInds;
layout (location = 3) in  vec4 boneWeights;

const int MAX_BONES = 200;

// Uniform variables
uniform mat4 viewProj;
uniform mat4 model;
uniform mat4 bones[MAX_BONES];

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. 
out vec3 fragNormal;


void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    vec4 temp_pos = boneWeights.x * bones[boneInds.x] * vec4(position, 1.0) +
                    boneWeights.y * bones[boneInds.y] * vec4(position, 1.0) +
                    boneWeights.z * bones[boneInds.z] * vec4(position, 1.0) +
                    boneWeights.w * bones[boneInds.w] * vec4(position, 1.0);
    gl_Position = viewProj * model * temp_pos;

    // TODO: This implementation has assumption about transformation matrix
    vec4 temp_norm = boneWeights.x * bones[boneInds.x] * vec4(normal, 0) +
                     boneWeights.y * bones[boneInds.y] * vec4(normal, 0) +
                     boneWeights.z * bones[boneInds.z] * vec4(normal, 0) +
                     boneWeights.w * bones[boneInds.w] * vec4(normal, 0);

    // for shading
	fragNormal = vec3(model * temp_norm);
}
