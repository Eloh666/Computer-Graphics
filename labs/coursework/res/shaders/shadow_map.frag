#version 450

layout(location = 0) in vec4 vertexPos;

layout(location = 0) out vec4 colour;

void main () {
	float range = vertexPos.x * vertexPos.x +  vertexPos.y * vertexPos.y +  vertexPos.z * vertexPos.z;
	range /= 1000000.0;
	colour = vec4(vec3(range), 1.0);
}