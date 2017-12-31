#version 330 core
layout (location = 0) in vec3 position;

out VS_OUT{
    vec3 position;
}vs_out;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vs_out.position = position;
    gl_Position =  projection * view * vec4(vs_out.position, 1.0);
}
