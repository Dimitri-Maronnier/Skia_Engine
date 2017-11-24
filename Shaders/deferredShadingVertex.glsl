#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoord;

out vec2 pass_textureCoord;

void main()
{
    pass_textureCoord = textureCoord;
    gl_Position = vec4(position, 1.0);
}
