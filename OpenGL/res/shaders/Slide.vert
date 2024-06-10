#version 330 core

layout(location = 0) in vec4 pos;

uniform vec2 u_Scale;

out vec4 position;

void main() {
	position = pos;
	gl_Position = vec4(pos.x * u_Scale.x, pos.y * u_Scale.y, pos.z, pos.w);
}