#version 330 core
out vec2 FragColor;
in vec2 textureCoord;

const float PI = 3.14159265359;

float RadicalInverse_VanDerCorpus(uint bits)
{
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10;
}
// ----------------------------------------------------------------------------
vec2 Hammersley(uint i, uint N)
{
        return vec2(float(i)/float(N), RadicalInverse_VanDerCorpus(i));
}
// ----------------------------------------------------------------------------
vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
	float a = roughness*roughness;
	
	float phi = 2.0 * PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
	
	// from spherical coordinates to cartesian coordinates - halfway vector
        vec3 HalfWay;
        HalfWay.x = cos(phi) * sinTheta;
        HalfWay.y = sin(phi) * sinTheta;
        HalfWay.z = cosTheta;
	
	// from tangent-space H vector to world-space sample vector
        vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent   = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);
	
        vec3 sampleVec = tangent * HalfWay.x + bitangent * HalfWay.y + N * HalfWay.z;
	return normalize(sampleVec);
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float nDotv, float roughness)
{

    float a = roughness;
    float k = (a * a) / 2.0;

    float nom   = nDotv;
    float denom = nDotv * (1.0 - k) + k;

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
vec2 IntegrateBRDF(float nDotv, float roughness)
{
    vec3 viewVector;
    viewVector.x = sqrt(1.0 - pow(nDotv,2.0));
    viewVector.y = 0.0;
    viewVector.z = nDotv;

    float u = 0.0;
    float v = 0.0;

    vec3 normal = vec3(0.0, 0.0, 1.0);
    
    const uint SAMPLE_COUNT = 1024u;
    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        vec2 Xi = Hammersley(i, SAMPLE_COUNT);
        vec3 H = ImportanceSampleGGX(Xi, normal, roughness);
        vec3 L = normalize(2.0 * dot(viewVector, H) * H - viewVector);

        float nDotL = max(L.z, 0.0);
        float nDotH = max(H.z, 0.0);
        float vDoth = max(dot(viewVector, H), 0.0);

        if(nDotL > 0.0)
        {
            float G = GeometrySmith(normal,viewVector, L, roughness);
            float G_Vis = (G * vDoth) / (nDotH * nDotv);
            float Fc = pow(1.0 - vDoth, 5.0);

            u += (1.0 - Fc) * G_Vis;
            v += Fc * G_Vis;
        }
    }
    u /= float(SAMPLE_COUNT);
    v /= float(SAMPLE_COUNT);
    return vec2(u, v);
}
// ----------------------------------------------------------------------------
void main() 
{
    vec2 integratedBRDF = IntegrateBRDF(textureCoord.x, textureCoord.y);
    FragColor = integratedBRDF;
}
