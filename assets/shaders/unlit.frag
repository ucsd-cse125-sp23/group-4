#version 330 core

in vec3 fragNormal;

// uniforms
uniform vec3 diffuseColor;

out vec4 fragColor;

void main()
{
	fragColor = vec4(diffuseColor, 1);
}