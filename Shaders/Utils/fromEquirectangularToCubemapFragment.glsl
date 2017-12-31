#version 330 core
in VS_OUT{
    vec3 position;
}fs_in;

uniform sampler2D equirectangularEnvMap;

vec2 sphericalUV(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= vec2(0.1591, 0.3183);
    uv += 0.5;
    return uv;
}

void main()
{
    vec3 color = texture(equirectangularEnvMap, sphericalUV(normalize(fs_in.position))).rgb;
    gl_FragColor = vec4(color, 1.0);
}
