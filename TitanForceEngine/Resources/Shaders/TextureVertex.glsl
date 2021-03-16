#version 410
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 colour;

out vec2 TexCoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightPos;

void main() {
	TexCoord = texCoords;
	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}