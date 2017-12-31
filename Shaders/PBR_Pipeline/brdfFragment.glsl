/**
*
*   Compute BRDF Lut according to Epic's Game work
*
**/
#version 330 core
in vec2 textureCoord;
out vec2 FragColor;
uniform uint nbSample = 1024u;
uniform uint type = 6u;

const float M_PI = 3.1415;


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
*   ImportanceSampleGGX
*/
vec3 ImportanceSampleGGX(vec2 Xi, vec3 normal, float roughness)
{
    float a = roughness*roughness; //Square Roughness for better looking result

    float Phi = 2.0 * M_PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

    vec3 halfWay = vec3(sinTheta * cos(Phi),sinTheta * sin(Phi),cosTheta);

    vec3 up = abs(normal.z) < 0.9 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);//Float Accurate
    vec3 Tangent = normalize(cross(up, normal));
    vec3 Bitangent = cross(normal, Tangent);


    return normalize(vec3(Tangent * halfWay.x + Bitangent * halfWay.y + normal * halfWay.z));
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
float GeometryKelemen(vec3 N, vec3 V, vec3 L,vec3 H)
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
    float a = roughness*roughness;
    float a2 = a*a;
    float num = 2*nDotV;
    float denum = nDotV * sqrt(a2 + (1 - a2)*nDotV*nDotV);
    return num/denum;
}

/*
*   SchlickGGX
*/
float Geometry_SchlickGGX(float nDotv, float roughness)
{

    float a = roughness*roughness;
    float k = (a * a) / 2.0;

    float num   = nDotv;
    float denum = nDotv * (1.0 - k) + k;

    return num / denum;
}

/*
*   Geometry_Smith : combine geometry shodowing and occlusion
*/
float Geometry_Smith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float nDotV = max(dot(N, V), 0.0);
    float nDotL = max(dot(N, L), 0.0);
    float oclude,shadowing;
    switch(type){
        case 5:
            oclude = GeometryGGX(nDotV, roughness);
            shadowing = GeometryGGX(nDotL, roughness);
            break;
        case 6:
            oclude = Geometry_SchlickGGX(nDotV, roughness);
            shadowing = Geometry_SchlickGGX(nDotL, roughness);
            break;
    }

    return oclude * shadowing;
}

vec2 IntegrateBRDF(float nDotv, float roughness)
{
    vec3 viewVector;
    viewVector.x = sqrt(1.0 - nDotv * nDotv);//sin
    viewVector.y = 0.0;
    viewVector.z = nDotv;//cos
    float u = 0.0;
    float v = 0.0;
    vec3 normal = vec3(0.0, 0.0, 1.0);
    for(uint i = 0u; i < nbSample; i++)
    {
        vec2 Xi = Hammersley(i, nbSample);
        vec3 halfWay = ImportanceSampleGGX(Xi, normal, roughness);
        vec3 lightVector = normalize(2.0 * dot(viewVector, halfWay) * halfWay - viewVector);

        float nDotL = max(lightVector.z, 0.0);
        float nDotH = max(halfWay.z, 0.0);
        float vDoth = max(dot(viewVector, halfWay), 0.0);

        if(nDotL > 0.0)
        {
            float G = Geometry_Smith(normal,viewVector, lightVector, roughness);
            switch(type){
                case 1://Implicit
                    G = GeometryImplicit(normal,viewVector, lightVector);
                    break;
                case 2://Neumann
                    G = GeometryNeumann(normal,viewVector, lightVector);
                    break;
                case 3://Cook-Torrance
                    G = GeometryCookTorrance(normal,viewVector, lightVector,halfWay);
                    break;
                case 4://Kelmen
                    G = GeometryKelemen(normal,viewVector, lightVector,halfWay);
                    break;
                default://Smith GGX/SchlickGGX
                    G = Geometry_Smith(normal,viewVector, lightVector, roughness);
                    break;
            }
            float G_Vis = (G * vDoth) / (nDotH * nDotv);
            float Fc = pow(1.0 - vDoth, 5.0);//Fresnel

            u += (1 - Fc) * G_Vis;
            v += Fc * G_Vis;
        }
    }

    return vec2(u, v)/float(nbSample);
}

void main() 
{
    vec2 integratedBRDF = IntegrateBRDF(textureCoord.x, textureCoord.y);
    FragColor = integratedBRDF;
}
