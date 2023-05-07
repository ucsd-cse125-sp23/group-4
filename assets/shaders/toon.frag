#version 330 core

in vec3 fragNormal;
in vec2 texCoord0;	// UV coordinates

uniform sampler2D gSampler;

// uniforms used for lighting
uniform vec3 AmbientColor = vec3(0.1);
uniform vec3 LightDirections[] = {
									normalize(vec3(1, 5, 2)),
									normalize(vec3(-5, -1, -3))
								 };
uniform vec3 LightColors[] = { vec3(1), vec3(0.6, 0.7, 1) * 0.4 };
uniform vec3 DiffuseColor;	// passed in from c++ side NOTE: you can also set the value here and then remove 
							// color from the c++ side

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{

	vec3 lightsum = vec3(0.0);

	for (int i = 0; i < 2; i++){
        lightsum += LightColors[i] * max(0, dot(LightDirections[i], fragNormal));
    }

	vec3 color;

	// Stepped lighting ramp
	if(length(lightsum) > 0.95) {
		color = DiffuseColor * 1.0;
	}
	else if(length(lightsum) > 0.5) {
		color = DiffuseColor * 0.6;
	}
	else if(length(lightsum) > 0.25) {
		color = DiffuseColor * 0.4;
	}
	else {
		color = DiffuseColor * 0.2;
	}


	// Compute irradiance (sum of ambient & direct lighting)
	vec3 irradiance = AmbientColor + lightsum;

	// Diffuse reflectance
	vec3 reflectance = irradiance * DiffuseColor;

	// sampled texture color
	vec4 texturedColor = texture2D(gSampler, texCoord0.st);

	// Gamma correction
	//fragColor = vec4(sqrt(reflectance)/* * vec3(texturedColor)*/, 1);
	fragColor = vec4(color, 1);
}