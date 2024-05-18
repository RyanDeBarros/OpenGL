#version 400 core

in vec4 position;

uniform double u_DeltaTime;
uniform double u_TotalTime;

layout(location = 0) out vec4 color;

void main() {
	vec2 npos = 0.5 * (position.xy + 1.0);
	float c = step(-0.5, position.x) * (1 - step(0.5, position.x));
	c *= step(-0.5, position.y) * (1 - step(0.5, position.y));
	color = vec4(1 - c);
}