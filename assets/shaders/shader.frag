#version 330 core

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

in vec3 fragNormal;

// uniforms used for lighting
uniform vec3 AmbientColor = vec3(0.1);
uniform vec3 LightDirections1 = normalize(vec3(1, 5, 2));
uniform vec3 LightDirections2 = normalize(vec3(-5, -1, -3));
uniform vec3 LightColors1 = vec3(1);
uniform vec3 LightColors2 = vec3(0.6, 0.7, 1) * 0.4;
uniform vec3 DiffuseColor;	// passed in from c++ side NOTE: you can also set the value here and then remove 
							// color from the c++ side

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{

	vec3 lightsum = vec3(0.0);
	lightsum += LightColors1 * max(0, dot(LightDirections1, fragNormal));
	lightsum += LightColors2 * max(0, dot(LightDirections2, fragNormal));

	// Compute irradiance (sum of ambient & direct lighting)
	vec3 irradiance = AmbientColor + lightsum;

	// Diffuse reflectance
	vec3 reflectance = irradiance * DiffuseColor;

	// Gamma correction
	fragColor = vec4(sqrt(reflectance), 1);
}