#version 440

// Model view projection matrix
uniform mat4 MVP;

layout (location = 0) in vec3 position;
layout(location = 1) out float distanceFromCam;

void main()
{
	gl_Position = vec4(position, 1.0);
}