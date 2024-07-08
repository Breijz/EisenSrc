#version 410 core

layout (location = 0) in vec2 texcoord;
layout (location = 1) in vec3 worldcoord;
layout (location = 2) in vec4 color;

out vec2 vTex;
out vec3 vCoord;
out vec4 vColor;

void main() {
	gl_Position = vec4(1, 1, 1, 1.0f);
	vTex = texcoord;
	vCoord = worldcoord;
	vColor = color;
}

