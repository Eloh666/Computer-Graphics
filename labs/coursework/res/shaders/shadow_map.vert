#version 450 

uniform mat4 LVP;

layout(location = 0) in vec3 position;

layout(location = 0) out vec4 vertexPos;

void main() {
	vertexPos = LVP * vec4(position, 1.0);
	gl_Position = vertexPos;
}