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
const float M_PI = 3.1415;

void main()
{
    vec3 normal = normalize(fs_in.position);//Get normal from world vector (tangent surface)
    vec3 irradianceValue= vec3(0.0);
    vec3 upWorld = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(upWorld, normal);
    vec3 up = cross(normal, right);
    float samplesCount = 0.0;
    for(float theta = 0.0;theta < 0.5 * M_PI;theta += delta){//azimuth
        for(float phi = 0.0; phi < 2.0 * M_PI; phi += delta ){//hemisphere

            samplesCount++;
            vec3 TangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            vec3 vector = TangentSample.x * right + TangentSample.y * up + TangentSample.z * normal;
            irradianceValue += texture(environmentMap, vector).rgb * cos(theta) * sin(theta);
        }
    }
    irradianceValue *= M_PI/float(samplesCount);

    gl_FragColor = vec4(irradianceValue, 1.0);
}
