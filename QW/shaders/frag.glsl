#version 410 core

in vec2 vTex;
in vec3 vCoord;
in vec4 vColor;

out vec4 FragColor;

void main() {
	if(vColor.w < 0.666f) {
		return;
	}

	FragColor = vColor;
}

