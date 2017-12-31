#version 330 core

in VS_OUT{
    vec3 position;
}fs_in;
  
uniform samplerCube environmentMap;
  
void main()
{
    vec3 color = texture(environmentMap, fs_in.position).rgb;

    // tonemapping HDR
    color = color / (color + vec3(1.0));
    //correction gamma
    color = pow(color, vec3(1.0/2.2));
  
    gl_FragColor = vec4(color, 1.0);
}
