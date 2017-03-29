
#version 440
// Model view projection matrix
uniform mat4 MVP;
uniform vec3 cameraPosition;

// Incoming value for the position
layout (location = 0) in vec3 position;
layout(location = 5) out float distanceFromCam;


// Main vertex shader function
void main()
{
	// Calculate screen position of vertex
	gl_Position = MVP * vec4(position, 1.0);
	distanceFromCam = distance( position, cameraPosition);
}