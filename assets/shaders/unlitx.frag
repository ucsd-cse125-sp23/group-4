#version 330 core

in vec3 fragNormal;
in vec2 texCoord0;	// UV coordinates

// uniforms
uniform vec3 diffuseColor;

uniform int renderMode;	// 0 = no texture, 1 = texture
uniform sampler2D gSampler;

out vec4 fragColor;

void main()
{
// sampled texture color
	vec4 texturedColor = texture2D(gSampler, texCoord0.st);
	if (renderMode == 0) {
		texturedColor = vec4(1);
	}

	if (texturedColor.a < 0.1) {
		discard;
	}

	fragColor = vec4(diffuseColor * vec3(texturedColor), 1);
}