#version 430 core
in vec2 pass_textureCoord;
out vec4 FragColor;

uniform vec3 lightPosition;
uniform vec3 lightTint;
uniform vec3 viewPos;

layout(binding=0) uniform samplerCube irradianceMap;
layout(binding=1) uniform samplerCube prefilterMap;
layout(binding=2) uniform sampler2D brdfLUT;

layout(binding=3) uniform sampler2D gPosition;
layout(binding=4) uniform sampler2D gNormal;
layout(binding=5) uniform sampler2D gAlbedoRoughness;
layout(binding=6) uniform sampler2D gMetalOcclusion;



void main(void){
    bool normalMapped = false;
    float roughness = texture2D(gAlbedoRoughness,pass_textureCoord).a;
    float metallic = texture2D(gMetalOcclusion,pass_textureCoord).r;
    float ao = texture2D(gMetalOcclusion,pass_textureCoord).b;
    vec4 normalValue = texture2D(gNormal,pass_textureCoord);
    vec3 worldPos = texture2D(gPosition,pass_textureCoord).rgb;



    vec3 albedo = pow(texture2D(gAlbedoRoughness,pass_textureCoord).rgb, vec3(2.2));
    vec3 unitLightVector;
    vec3 unitVectorToCamera;
    vec3 unitNormal;

    vec3 toLightVector = lightPosition - worldPos;
    vec3 toLightVectorTangent;
    vec3 toCameraVector = viewPos - worldPos;
    vec3 toCameraVectorTangent;


    unitLightVector = normalize(toLightVectorTangent);
    unitVectorToCamera = normalize(toCameraVectorTangent);
    unitNormal = normalize(normalValue.rgb);


   vec3 N = unitNormal;

   vec3 V = normalize(toCameraVector);
   vec3 R = reflect(-V, N);


   vec3 color = vec4(1.0);

   // HDR tonemapping
   color = color / (color + vec3(1.0));
   // gamma correct
   color = pow(color, vec3(1.0/2.2));

   FragColor = vec4(color,1.0);
}
