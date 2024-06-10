#version 400 core

const float PI = 3.14159265;
const int numPoints = 5;
const float largeRadius = 1.0;
const float smallRadius = 0.5;

in vec4 position;

uniform double u_TotalTime;

layout(location = 0) out vec4 color;

void main() {
	float wideAngle = 2 * PI / numPoints;
	float angle01;
	if (position.x == 0)
		angle01 = mod(PI * 0.5, wideAngle) / wideAngle;
	else
		angle01 = mod(atan(position.y, position.x), wideAngle) / wideAngle;
	float stepBase = 2 * (largeRadius - smallRadius) * abs(angle01 - 0.5) + smallRadius;
	//stepBase = abs(angle01 - 0.5) + 0.5;
	
	//stepBase = cos(angle01 * length(position.xy));

	float beta = 1 - 2 * abs(angle01 - 0.5);
	float dis = pow(largeRadius, 2) * (pow(beta, 2) - pow(sin(PI * beta / numPoints), 2)) + pow(beta, 2) * pow(smallRadius, 2) - 2 * pow(beta, 2) * smallRadius * largeRadius * cos(PI / numPoints);
	stepBase = largeRadius * cos(PI * beta / numPoints) - sqrt(dis);

	float strength = step(stepBase, length(position.xy));
	color = vec4(vec3(strength), 1.0);
}