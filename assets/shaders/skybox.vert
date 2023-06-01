#version 330 core

layout (location = 0) in vec3 position;

// Uniform variables
uniform mat4 viewProj;

out vec3 texCoords;

void main()
{
    // proj * view * model * p
    vec4 screenpos = viewProj * vec4(position, 1.0);

    // ensure max depth (1.0) with pos.w
    gl_Position = vec4(screenpos.x, screenpos.y, screenpos.w, screenpos.w);
    texCoords = vec3(position.x, position.y, -position.z);
}