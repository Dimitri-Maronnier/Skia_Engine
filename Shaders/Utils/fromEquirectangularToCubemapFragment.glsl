#version 330 core
in VS_OUT{
    vec3 position;
}fs_in;

uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{		
    vec2 uv = SampleSphericalMap(normalize(fs_in.position));
    vec3 color = texture(equirectangularMap, uv).rgb;
    
    gl_FragColor = vec4(color, 1.0);
}
