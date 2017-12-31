#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;

out VS_OUT{
    vec3 position;
}vs_out;

void main()
{
    vs_out.position = position;

    mat4 newView = mat4(mat3(view)); // no need translation for a skyBox
    vec4 clipPos = projection * newView * vec4(vs_out.position, 1.0);

    gl_Position = clipPos.xyww;
}
