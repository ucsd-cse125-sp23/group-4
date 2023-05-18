#version 330 core

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 texCoord0;	// UV coordinates

uniform mat4 view;      // from world coord to eye coord

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

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

vec3 unitDir (vec4 p, vec4 q){
    return normalize(p.w * q.xyz - q.w * p.xyz);
}

void main()
{

	vec4 posC = view * vec4(fragPosition, 1);
    vec3 viewdir = unitDir(posC, vec4(0.0, 0.0, 0.0, 1.0));  // unit direction towards viewer

	vec3 lightsum = vec3(0.0);

	for (int i = 0; i < 2; i++){
        lightsum += LightColors[i] * max(0, dot(LightDirections[i], fragNormal));
    }

	vec3 halfwayv = normalize(viewdir + LightDirections[0]);  // hj = half-way direction between v to lj

	lightsum += pow(max(dot(normalize(fragNormal), halfwayv), 0.0), shininess);

	vec3 color;

	lightsum = sqrt(lightsum);

	// Stepped lighting ramp
	if(length(lightsum) > 1.85) {
		color = diffuseColor * 1.2;
	}
	else if(length(lightsum) > 0.70) {
		color = diffuseColor * 1.0;
	}
	else {
		color = diffuseColor * 0.9;
	}


	// Compute irradiance (sum of ambient & direct lighting)
	vec3 irradiance = ambientColor + lightsum;

	// Diffuse reflectance
	vec3 reflectance = irradiance * color;

	// sampled texture color
	vec4 texturedColor = texture2D(gSampler, texCoord0.st);

	// Gamma correction
	fragColor = vec4(sqrt(reflectance)/* * vec3(texturedColor)*/, 1);
	//fragColor = vec4(color, 1);
}