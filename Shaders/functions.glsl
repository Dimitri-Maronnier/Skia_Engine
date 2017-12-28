
const float PI = 3.14159265359;

//LearnOpengl parallaxMapping
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
*   Blin-Phong
*/
float DistributionBlinPhong(vec3 N, vec3 H, float roughness)
{
    float a2 = roughness*roughness;
    float power = 2/a2 - 2;
    float nDotH = max(dot(N, H), 0.0);
    float right = 1/(PI*a2);
    float left = pow(nDotH,power);
    return right*left;
}

/*
*   Beckmann
*/
float DistributionBeckmann(vec3 N, vec3 H, float roughness)
{
    float a2 = roughness*roughness;
    float nDotH = max(dot(N, H), 0.0);
    float denum = PI*a2*(nDotH*nDotH*nDotH*nDotH);
    float numExp = nDotH*nDotH -1;
    float denumExp = a2 * nDotH*nDotH;
    float Exp = exp(numExp/denumExp);
    return 1/denum * Exp;
}

/*
*   GGX (Trowbridge-Reitz) : We adopted Disney's solution about squared twice the roughness
*/
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float nDotH = max(dot(N, H), 0.0);
    float nDotH2 = nDotH*nDotH;

    float num   = a2;
    float denum = (nDotH2 * (a2 - 1.0) + 1.0);
    denum = PI * denum * denum;

    return num / denum;
}

/*
*   Geometry functions : self shadowing/occlusion propery of microfacets
*/

/*
*   Implicit
*/

float GeometryImplicit(vec3 N, vec3 V, vec3 L)
{
    float nDotV = max(dot(N, V), 0.0);
    float nDotL = max(dot(N, L), 0.0);
    return nDotV * nDotL;
}

/*
*   Neumann
*/
float GeometryNeumann(vec3 N, vec3 V, vec3 L)
{
    float nDotV = max(dot(N, V), 0.0);
    float nDotL = max(dot(N, L), 0.0);
    float num = nDotV * nDotL;
    float denum = max(nDotV,nDotL);
    return num/denum;
}

/*
*   Cook-Torrance
*/
float GeometryCookTorrance(vec3 N, vec3 V, vec3 L,vec3 H)
{
    float nDotV = max(dot(N, V), 0.0);
    float nDotL = max(dot(N, L), 0.0);
    float nDotH = max(dot(N, H), 0.0);
    float vDotH = max(dot(V, H), 0.0);
    float A = (2*nDotH*nDotV)/vDotH;
    float B = (2*nDotH*nDotL)/vDotH;
    return min(min(1,A),B);
}

/*
*   Kelemen
*/
float Geometry(vec3 N, vec3 V, vec3 L,vec3 H)
{
    float nDotV = max(dot(N, V), 0.0);
    float nDotL = max(dot(N, L), 0.0);
    float vDotH = max(dot(V, H), 0.0);
    float num = nDotL*nDotV;
    float denum = vDotH*vDotH;
    return num/denum;

}

/*
*   GGX
*/
float GeometryGGX(float nDotV, float roughness)
{
    float a2 = roughness*roughness;
    float num = 2*nDotV;
    float denum = nDotV * sqrt(a2 + (1 - a2)*nDotV*nDotV);
    return num/denum;
}

/*
*   SchlickGGX
*/
float GeometrySchlickGGX(float nDotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = nDotV;
    float denum = nDotV * (1.0 - k) + k;

    return num / denum;
}

/*
*   Geometry_Smith : combine geometry shodowing and occlusion
*/
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float nDotV = max(dot(N, V), 0.0);
    float nDotL = max(dot(N, L), 0.0);
    float occlude = GeometrySchlickGGX(nDotV, roughness);
    float shadow = GeometrySchlickGGX(nDotL, roughness);

    return occlude * shadow;
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

