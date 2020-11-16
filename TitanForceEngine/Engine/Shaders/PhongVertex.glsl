#version 410
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 colour;

out vec3 vertNormal;
out vec3 lightDir;
out vec3 eyeDir; 
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightPos;

void main() {
	vertNormal = normalize(normalMatrix * normal); /// Rotate the normal to the correct orientation 
	vec3 vertPos = vec3(viewMatrix * modelMatrix * vec4(position, 1.0) ); /// This is the position of the vertex from the origin
	vec3 vertDir = normalize(vertPos);
	eyeDir = -vertDir;
	lightDir = normalize(lightPos - vertPos); /// Create the light direction. I do the math with in class 
	
	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0); 
}