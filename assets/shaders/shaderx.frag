#version 330 core

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 texCoord0;	// UV coordinates

uniform int renderMode;	// 0 = no texture, 1 = texture
uniform sampler2D gSampler;

// uniforms used for lighting
uniform vec3 LightDirections[] = {
									normalize(vec3(1, 5, 2)),
									normalize(vec3(-5, -1, -3))
								 };
uniform vec3 LightColors[] = { vec3(1), vec3(0.6, 0.7, 1) * 0.4 };

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 emissionColor;
uniform float shininess;

// output color
out vec4 fragColor;

vec3 unitDir (vec4 p, vec4 q){
    return normalize(p.w * q.xyz - q.w * p.xyz);
}

void main()
{

	vec4 posC = vec4(fragPosition, 1);
    vec3 viewdir = unitDir(posC, vec4(0.0, 0.0, 0.0, 1.0));  // unit direction towards viewer

	vec3 lightsum = vec3(0.0);

	for (int i = 0; i < 2; i++){
        lightsum += LightColors[i] * max(0, dot(LightDirections[i], fragNormal));
    }

	// Compute irradiance (sum of ambient & direct lighting)
	vec3 irradiance = ambientColor + lightsum;

	// Diffuse reflectance
	vec3 reflectance = irradiance * diffuseColor;

	vec3 halfwayv = normalize(viewdir + LightDirections[0]);  // hj = half-way direction between v to lj

	// Add in specular
	reflectance += specularColor * pow(max(dot(fragNormal, halfwayv), 0.0), shininess);

	// sampled texture color
	vec4 texturedColor = texture2D(gSampler, texCoord0.st);
	if (renderMode == 0) {
		texturedColor = vec4(1);
	}

	// Gamma correction
	fragColor = vec4(emissionColor + sqrt(reflectance) * vec3(texturedColor), 1);
	//fragColor = texturedColor;	// testing
}