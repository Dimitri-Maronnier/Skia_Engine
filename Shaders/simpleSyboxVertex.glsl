#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;

out vec3 localPos;

void main()
{
    localPos = position;

    mat4 rotView = mat4(mat3(view)); // no need translation for a skyBox
    vec4 clipPos = projection * rotView * vec4(localPos, 1.0);

    gl_Position = clipPos.xyww;
}
