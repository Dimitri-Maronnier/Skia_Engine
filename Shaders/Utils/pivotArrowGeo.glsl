#version 330 core

layout(points) in;
layout (line_strip, max_vertices=2) out;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform vec3 viewPos;
uniform vec3 a;
uniform vec3 b;

void main(void)
{
    mat4 viewMatrixMod = viewMatrix;
    vec4 p = projectionMatrix * viewMatrixMod * vec4(a,0.0);
    float w = p.w;

    //p /= w;
    gl_Position = p;

    EmitVertex();
    p = projectionMatrix * viewMatrixMod *  (vec4(b,0.0)*500);
    float d = distance(p.xyz,viewPos)/1000.0;
    w = p.w;

    //p /= w;
    gl_Position = p;
    EmitVertex();

    EndPrimitive();

}
