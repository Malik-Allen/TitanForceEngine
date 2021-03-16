#version 410

in vec2 TexCoord;

uniform sampler2D ourTexture;

out vec4 fragColor;

void main() {
	fragColor = texture(ourTexture, TexCoord);
}

