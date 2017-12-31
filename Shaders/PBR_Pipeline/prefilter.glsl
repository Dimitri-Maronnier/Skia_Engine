/**
*
*   Compute the prefilter (first part of Epic's game is Split Sum approximation)
*
*/
#version 330 core
in VS_OUT{
    vec3 position;
}fs_in;

uniform samplerCube environmentMap;
uniform float roughness;
uniform uint nbSample = 1024u;
uniform uint type = 3u;

const float M_PI = 3.1415;
uniform float resolution = 512.0;


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
float DistributionBlinPhong(vec3 N, vec3 H)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float power = 2/a2 - 2;
    float nDotH = max(dot(N, H), 0.0);
    float right = 1/(M_PI*a2);
    float left = pow(nDotH,power);
    return right*left;
}

/*
*   Beckmann
*/
float DistributionBeckmann(vec3 N, vec3 H)
{
    float a = roughness*roughness;
    float a2 = a*a;
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
float DistributionGGX(vec3 N, vec3 H)
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
*   ImportanceSample
*/
vec3 ImportanceSample(vec2 Xi, vec3 normal)
{
    float a = roughness*roughness;

    float phi = 2.0 * M_PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

    vec3 halfWay = vec3(sinTheta * cos(phi),sinTheta * sin(phi),cosTheta);

    vec3 up = abs(normal.z) < 0.9 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 Tangent = normalize(cross(up, normal));
    vec3 Bitangent = cross(normal, Tangent);

    vec3 sampleVec = Tangent * halfWay.x + Bitangent * halfWay.y + normal * halfWay.z;
    return normalize(sampleVec);
}

vec3 prefilterEnvMap(vec3 normal)
{
    // Epic Games is simplification assume than normal=reflectVector=viewVector (cannot handle Grazing Specular Reflections)
    vec3 reflectVector = normal;
    vec3 viewVector = reflectVector;

    vec3 prefilteredColor = vec3(0.0);
    float totalWeight = 0.0;

    for(uint i = 0u; i < nbSample; i++)
    {

        vec2 Xi = Hammersley(i, nbSample);
        vec3 halfWay = ImportanceSample(Xi, normal);
        vec3 lightVector  = normalize(2.0 * dot(viewVector, halfWay) * halfWay - viewVector);

        float ndotL = max(dot(normal, lightVector), 0.0);
        float ndotH = max(dot(normal, halfWay), 0.0);
        float hdotV = max(dot(halfWay, viewVector), 0.0);
        if(ndotL > 0.0)
        {
            // get mip map level based on roughness
            float D;
            switch(type){
                case 1://BlinPhong
                    D = DistributionBlinPhong(normal, halfWay);
                    break;
                case 2://Beckmann
                    D = DistributionBeckmann(normal, halfWay);
                    break;
                case 3://GGX
                    D = DistributionGGX(normal, halfWay);
                    break;
                default://GGX
                    D = DistributionGGX(normal, halfWay);
                    break;
            }
            /*vec3 FC = pow( 1 - hdotV, 5 );
            vec3 F = (1 - Fc) * SpecularColor + Fc;*/

            /*Chetan Jags's method to reduce artifact*/
            float pdf = D * ndotH / (4.0 * hdotV + 0.0001);//Prevent zero division
            float saTexel  = 4.0 * M_PI / (6.0 * resolution * resolution);
            float saSample = 1.0 / (float(nbSample) * pdf + 0.0001);//Prevent zero division
            float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);

            prefilteredColor += textureLod(environmentMap, lightVector, mipLevel).rgb * ndotL;

            totalWeight += ndotL;
        }
    }

    return prefilteredColor / totalWeight;

}

void main()
{		
    vec3 N = normalize(fs_in.position);
    

    gl_FragColor = vec4(prefilterEnvMap(N), 1.0);
}
