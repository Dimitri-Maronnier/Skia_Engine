#version 420 core

in vec3 position;
in vec2 textureCoord;
in vec3 normal;
in vec3 tangent;

out VS_OUT{
    vec3 normal;
    vec3 toLightVector;
    vec2 textureCoord;
}vs_out;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPosition;

void main(void)
{
   /*const vec4 vertices[] = vec4[](vec4( 1, -0, -1, 1.0),
                                    vec4(-1, -1, -1, 1.0),
                                    vec4( 0.0,  1, -1, 1.0));*/
    vec4 worldPosition = modelMatrix * vec4(position,1.0);
    gl_Position = projectionMatrix * viewMatrix * worldPosition;
    vs_out.toLightVector =  (lightPosition - worldPosition.xyz);
    vs_out.normal = (modelMatrix*vec4(normal,0.0)).xyz;
    vs_out.textureCoord = textureCoord;
}
