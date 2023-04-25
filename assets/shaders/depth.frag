#version 330 core

in vec4 position; // raw position in the model coord
in vec3 normal;   // raw normal in the model coord

uniform mat4 modelview; // from model coord to eye coord
uniform mat4 view;      // from world coord to eye coord

// Output the frag color
out vec4 fragColor;


float near = 0.5;
float far  = 250.0;
  
float remapDepth(float depth)
{
    // Needs to be divided by far to be 0.0 - 1.0
    float z = (depth * 2.0) - 1.0;
    return (2.0 * far * near) / (far + near - z * (far - near));	
}

void main (void){
    // Depth shading
    float depth = remapDepth(gl_FragCoord.z) / (far - near);
    fragColor = vec4(vec3(depth), 1.0);

    // If we want, we can set the depth manually to make the depth buffer not just white
    //gl_FragDepth = depth;
}
