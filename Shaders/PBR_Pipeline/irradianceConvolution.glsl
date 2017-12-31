/**
*
*   Compute Irradiance Convultion, the diffuse part of Reflectance Equation
*
*/
#version 330 core
in VS_OUT{
    vec3 position;
}fs_in;

uniform samplerCube environmentMap;
uniform float delta = 0.02;

const float M_PI = 3.14159265359;

void main()
{
    vec3 N = normalize(fs_in.position);//Get normal from world vector (tangent surface)
    vec3 irradiance= vec3(0.0);
    // tangent space calculation from origin point
    vec3 upWorld = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(upWorld, N);
    vec3 up = cross(N, right);
    float samplesCount = 0.0;
    for(float phi = 0.0; phi < 2.0 * M_PI; phi += delta){//hemisphere
        for(float theta = 0.0; theta < 0.5 * M_PI; theta += delta){//zenith
            // spherical coords to cartesian coords
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            vec3 vector = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;
            irradiance += texture(environmentMap, vector).rgb * cos(theta) * sin(theta);
            samplesCount++;
        }
    }
    irradiance *= M_PI/float(samplesCount);

    gl_FragColor = vec4(irradiance, 1.0);
}
