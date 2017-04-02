
#version 440

// Incoming value for the position
layout (location = 0) in vec3 position;


// Main vertex shader function
void main()
{
	// Calculate screen position of vertex
	gl_Position = vec4(position, 1.0);
}