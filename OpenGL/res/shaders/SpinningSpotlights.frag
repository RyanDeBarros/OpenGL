#version 400 core

const float PI = 3.14159265;
const int numPoints = 5;
const float largeRadius = 10.0;
const float smallRadius = 1.0;
const float speed = 0.5;

in vec4 position;

uniform double u_TotalTime;

layout(location = 0) out vec4 color;

void main() {
	float wideAngle = 2 * PI / numPoints;
	float angle01;
	if (position.x == 0)
		angle01 = mod(PI * 0.5 + float(u_TotalTime) * speed, wideAngle) / wideAngle;
	else
		angle01 = mod(atan(position.y, position.x) + float(u_TotalTime) * speed, wideAngle) / wideAngle;
	color = vec4(angle01, 1 - angle01, 0.0, 1.0);
}