#version 440

uniform vec4 colour;
layout(location = 5) in float distanceFromCam;

// Outgoing colour for the shader
layout (location = 0) out vec4 out_colour;

void main()
{
	out_colour = colour;
}