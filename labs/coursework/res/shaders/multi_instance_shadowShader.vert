#version 450 

// The camera projection
uniform mat4 projection;
// The camera view
uniform mat4 view;

uniform mat4 MVP;

layout(location = 0) in vec3 position;
layout (location = 5) in mat4 instanceMatrix;

layout(location = 0) out vec4 vertexPos;

void main() {
	vertexPos = projection * view * instanceMatrix * vec4(position, 1.0f);
	gl_Position = vertexPos;
}