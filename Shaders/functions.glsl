
const float PI = 3.14159265359;
vec2 parallaxMapping(vec2 texCoords,sampler2D depthMap,float heightScale, bool discardOver,float minLayers,float maxLayers)
{
    vec3 viewDir = normalize(fs_in.toCameraVectorTangent);
    // number of depth layers
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * heightScale;
    vec2 deltaTexCoords = P / numLayers;

    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = 1-texture2D(depthMap, currentTexCoords).r;

    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = 1-texture2D(depthMap, currentTexCoords).r;
        // get depth of next layer
        currentLayerDepth += layerDepth;
    }

    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = 1-texture2D(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;

    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    if(discardOver && (finalTexCoords.x > 1.0 || finalTexCoords.y > 1.0 || finalTexCoords.x < 0.0 || finalTexCoords.y < 0.0))
        discard;
    return finalTexCoords;
}

/*
*   Normal Distribution Functions : Approximate the ratio of microfacets aligned to some HalWay vector
*/

/*
*   GGX (Trowbridge-Reitz) : We adopted Disney's to squared the roughness
*/
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

/*
*   Geometry functions : self shadowing/occlusion propery of microfacets
*/

/*
*   GGX
*/
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

/*
*   Geometry_Smith : combine geometry shodowing and occlusion
*/
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

/*
*   Fresnel functions :  The Fresnel calulate ratio of surface reflection at different surface angles
*/

/*
*   Schilck : F0 is base reflectivity of material
*/
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

/*
*   Schilck with roughness : F0 is base reflectivity of material
*/
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}


//----------------------Append Function-----------------------------------------//
//------------------------------------------------------------------------------
vec2 append(float a,float b){
    return vec2(a,b);
}

vec3 append(vec2 a,float b){
    return vec3(a,b);
}

vec3 append(float a,vec2 b){
    return vec3(a,b);
}

vec4 append (float a, vec3 b){
    return vec4(a,b);
}

vec4 append (vec3 a, float b){
    return vec4(a,b);
}

vec4 append (vec2 a, vec2 b){
    return vec4(a,b);
}

