#version 330 core
out vec4 FragColor;
in vec3 worldPosition;

uniform samplerCube environmentMap;

const float M_PI = 3.14159265359;

void main()
{		

    vec3 N = normalize(worldPosition);//Get normal from world vector (tangent surface)

    vec3 irradianceValue = vec3(0.0);
    
    // tangent space calculation from origin point
    vec3 upWorld = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(upWorld, N);
    vec3 up = cross(N, right);
       
    float delta = 0.03;
    float samplesCount = 0.0;
    for(float phi = 0.0; phi < 2.0 * M_PI; phi += delta)//hemisphere
    {
        for(float theta = 0.0; theta < 0.5 * M_PI; theta += delta)//zenith
        {
            // spherical coords to cartesian coords (in tangent space)
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            // tangent space to world space
            vec3 vector = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

            irradianceValue += texture(environmentMap, vector).rgb * cos(theta) * sin(theta);
            samplesCount++;
        }
    }
    irradianceValue = M_PI * irradianceValue * (1.0 / float(samplesCount));
    
    FragColor = vec4(irradianceValue, 1.0);
}
