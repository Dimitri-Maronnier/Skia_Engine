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


const float PI = 3.14159265359;

// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------

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

   // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
   // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
   vec3 F0 = vec3(0.04);
   F0 = mix(F0, albedo, metallic);

   // reflectance equation
   vec3 Lo = vec3(0.0);

   // calculate per-light radiance
   vec3 L = normalize(toLightVector);
   vec3 H = normalize(V + L);
   float distance = length(toLightVector);
   float attenuation = 1.0 / (distance * distance);
   vec3 radiance = lightTint;

   // Cook-Torrance BRDF
   float NDF = DistributionGGX(N, H, roughness);
   float G   = GeometrySmith(N, V, L, roughness);
   vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

   vec3 nominator    = NDF * G * F;
   float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
   vec3 specular = nominator / denominator;

   // kS is equal to Fresnel
   vec3 kS = F;
   // for energy conservation, the diffuse and specular light can't
   // be above 1.0 (unless the surface emits light); to preserve this
   // relationship the diffuse component (kD) should equal 1.0 - kS.
   vec3 kD = vec3(1.0) - kS;
   // multiply kD by the inverse metalness such that only non-metals
   // have diffuse lighting, or a linear blend if partly metal (pure metals
   // have no diffuse light).
   kD *= 1.0 - metallic;

   // scale light by NdotL
   float NdotL = max(dot(N, L), 0.0);

   // add to outgoing radiance Lo
   Lo += (kD * albedo / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again


   // ambient lighting (we now use IBL as the ambient term)
   F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

   kS = F;
   kD = 1.0 - kS;
   kD *= 1.0 - metallic;

   vec3 irradiance = texture(irradianceMap, N).rgb;
   vec3 diffuse      = irradiance * albedo;

   // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
   const float MAX_REFLECTION_LOD = 4.0;
   vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;
   vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
   specular = prefilteredColor * (F * brdf.x + brdf.y);

   vec3 ambient = (kD * diffuse + specular) * ao;

   vec3 color = ambient + Lo;

   // HDR tonemapping
   color = color / (color + vec3(1.0));
   // gamma correct
   color = pow(color, vec3(1.0/2.2));

   FragColor = vec4(color,1.0);
}
