#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out VS_OUT{
    vec3 position;
}vs_out;

void main()
{
    vs_out.position = position;

    mat4 newView = mat4(mat3(viewMatrix)); // no need translation for a skyBox
    vec4 clipPosition = projectionMatrix * newView * vec4(vs_out.position, 1.0);

    gl_Position = clipPosition.xyww;
}
