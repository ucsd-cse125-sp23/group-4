#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord; // textures!

// Uniform variables
uniform mat4 viewProj;
uniform mat4 model;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 texCoord0;


void main()
{
    // proj * view * model * p
    gl_Position = viewProj * model * vec4(position, 1.0);

    fragPosition = vec3(model * vec4(position, 0));
	fragNormal = vec3(model * vec4(normal, 0));

    // for textures (if not binded, no error)
    texCoord0 = texcoord;
}