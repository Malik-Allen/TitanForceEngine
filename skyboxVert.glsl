#version 410
layout (location = 0) in vec4 vVertex;

out vec3 TexCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
    TexCoords = vVertex.xyz;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vVertex;
}