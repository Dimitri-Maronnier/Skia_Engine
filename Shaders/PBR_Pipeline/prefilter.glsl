/**
*
*   Compute the prefilter (first part of Epic's game is Split Sum approximation)
*
*/
#version 330 core
out vec4 FragColor;
in vec3 worldPosition;

uniform samplerCube environmentMap;
uniform float roughness;

const float M_PI = 3.14159265359;
const float Resolution = 512.0; //TODO pass Uniform

/*
*   http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
*   Ven Der Corpus : mirrors a decimal binary around decimal representation
*/
float RadicalInverse_VanDerCorpus(uint bits)
{
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10;
}

/*
*   Hammersley : generate low-discrepancy sample i over N
*/
vec2 Hammersley(uint i, uint N)
{
        return vec2(float(i)/float(N), RadicalInverse_VanDerCorpus(i));
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
    float right = 1/(M_PI*a2);
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
    float denum = M_PI*a2*(nDotH*nDotH*nDotH*nDotH);
    float numExp = nDotH*nDotH -1;
    float denumExp = a2 * nDotH*nDotH;
    float Exp = exp(numExp/denumExp);
    return 1/denum * Exp;
}

/*
*   GGX (Trowbridge-Reitz) : We adopted Disney's to squared the roughness
*/
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float ndotH = max(dot(N, H), 0.0);
    float ndotH2 = ndotH*ndotH;

    float numerator   = a2;
    float denum = (ndotH2 * (a2 - 1.0) + 1.0);
    denum = M_PI * denum * denum;

    return numerator / denum;
}

/*
*   ImportanceSampleGGX
*/
vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
	float a = roughness*roughness;
	
        float phi = 2.0 * M_PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
	

        vec3 HalfWay;
        HalfWay.x = cos(phi) * sinTheta;
        HalfWay.y = sin(phi) * sinTheta;
        HalfWay.z = cosTheta;
	

        vec3 up          = abs(N.z) < 0.9 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent   = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);
	
        vec3 sampleVec = tangent * HalfWay.x + bitangent * HalfWay.y + N * HalfWay.z;
	return normalize(sampleVec);
}

void main()
{		
    vec3 N = normalize(worldPosition);
    
    // Epic Games is simplification assume than N=R=V (cannot handle Grazing Specular Reflections)
    vec3 R = N;
    vec3 V = R;

    const uint NUM_SAMPLE = 1024u;
    vec3 prefilteredColor = vec3(0.0);
    float totalWeight = 0.0;
    
    for(uint i = 0u; i < NUM_SAMPLE; ++i)
    {

        vec2 Xi = Hammersley(i, NUM_SAMPLE);
        vec3 H = ImportanceSampleGGX(Xi, N, roughness);
        vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);
        if(NdotL > 0.0)
        {
            // get mip map level based on roughness/pdf
            float D   = DistributionGGX(N, H, roughness);
            float NdotH = max(dot(N, H), 0.0);
            float HdotV = max(dot(H, V), 0.0);
            float pdf = D * NdotH / (4.0 * HdotV) + 0.0001; 

            float saTexel  = 4.0 * M_PI / (6.0 * Resolution * Resolution);
            float saSample = 1.0 / (float(NUM_SAMPLE) * pdf + 0.0001);

            float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel); 
            
            prefilteredColor += textureLod(environmentMap, L, mipLevel).rgb * NdotL;
            totalWeight      += NdotL;
        }
    }

    prefilteredColor = prefilteredColor / totalWeight;

    FragColor = vec4(prefilteredColor, 1.0);
}
