#version 330 core

in VS_OUT{
    vec3 normal;
    vec3 toLightVector;
    vec2 textureCoord;
}fs_in;

out vec4 color;

uniform sampler2D diffuseMap;

void main(void)
{
    vec3 unitLightVector = normalize(fs_in.toLightVector);
    vec3 unitNormal = normalize(fs_in.normal);
    float nDotl = dot(unitNormal,unitLightVector);
    float brightness = max(nDotl,0.5);
    color = vec4(1)*brightness;
}
