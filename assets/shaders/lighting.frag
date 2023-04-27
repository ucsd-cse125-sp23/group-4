#version 330 core

in vec4 position; // raw position in the model coord
in vec3 normal;   // raw normal in the model coord

uniform mat4 modelview; // from model coord to eye coord
uniform mat4 view;      // from world coord to eye coord

// Material parameters
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 emision;
uniform float shininess;

// Light source parameters
const int maximal_allowed_lights = 10;
uniform int nlights;
uniform vec4 lightpositions[ maximal_allowed_lights ];
uniform vec4 lightcolors[ maximal_allowed_lights ];

// Shadow computations
uniform sampler2D shadowMap;
in vec4 fragLight; // Additional information in light space

// Output the frag color
out vec4 fragColor;


vec3 dehomog(vec4 p){
    return p.xyz / p.w;
}

vec3 unitDir (vec4 p, vec4 q){
    return normalize(p.w * q.xyz - q.w * p.xyz);
}

void main (void){
    // Lighting computations
    // NOTE: uses camera frame/view space, not world space

    vec4 sum = vec4(0.0);

    vec3 nC = transpose(inverse(mat3(modelview))) * normal;
    nC = normalize(nC);
    vec4 posC = modelview * position;
    vec3 vdirC = unitDir(posC, vec4(0.0, 0.0, 0.0, 1.0));  // v = unit direction towards viewer

    // Calculate texture coordinates from light space position
    vec3 fragCoord = fragLight.xyz / fragLight.w;
    fragCoord = fragCoord * 0.5 + 0.5;  // Set to range 0 1

    // Sample depth value from shadow map
    float depthSampled = texture( shadowMap, fragCoord.xy ).r;
    float depthFrag = fragCoord.z;

    for (int j = 0; j < nlights; j++){
        vec4 lposC = view * lightpositions[j];

        vec3 ldirC = unitDir(posC, lposC);  // lj = direction to light
        vec3 hdirC = normalize(vdirC + ldirC);  // hj = half-way direction between v to lj

        float bias = max(0.00005 * (1.0 - dot(nC, ldirC)), 0.000005);
        float hasShadow = depthFrag - bias > depthSampled ? 1.0 : 0.0;
        // shadowM: 0 means we're in shadow
        // If this isn't the 0th light, ignore shadows
        float shadowM = dot(nC, ldirC) < 0.0 || j > 0 ? 1.0 : 1.0 - hasShadow;

        // Make a spotlight effect by checking edges of fragCoord
        float lightEdge = fragCoord.x > 1.0 || fragCoord.x < 0.0 || fragCoord.y > 1.0 || fragCoord.y < 0.0 ? 0.0 : 1.0;

        sum += (ambient + (diffuse * max(dot(nC, ldirC), 0.0) + specular * pow(max(dot(nC, hdirC), 0.0), shininess)) * shadowM * lightEdge) * lightcolors[j];
    }

    fragColor = emision + sum;

    // For debugging (probably after setting gl_FragDepth manually)
    //fragColor = vec4(vec3(depthSampled), 1.f);
    //fragColor = vec4(vec3(fragCoord.xyz), 1.f);
}
