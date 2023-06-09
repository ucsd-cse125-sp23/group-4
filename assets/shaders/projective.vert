// ((((not used)))) just a reference
# version 330 core

layout (location = 0) in vec3 vertex_position;  // raw position in the model coord
layout (location = 1) in vec3 vertex_normal;    // raw normal in the model coord

uniform mat4 modelview;
uniform mat4 model;
uniform mat4 projection;

uniform mat4 lightView;
uniform mat4 lightProj;

out vec4 position;
out vec3 normal;
out vec4 fragLight;


void main(){
    // proj * view * model * p
    gl_Position = projection * modelview * vec4( vertex_position, 1.0f );
    // forward the raw position and normal in the model coord to frag shader
    position = vec4(vertex_position, 1.0f );
    normal = vertex_normal;

    fragLight = lightProj * lightView * model * vec4( vertex_position, 1.0f );
}
