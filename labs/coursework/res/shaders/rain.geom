#version 440

layout (location = 0) in vec2 texCoords[];
layout (location = 0) in float distanceFromCam[];

layout (location = 0) out vec2 texCoordOut;
layout (location = 1) out float distanceFromCamera;


layout(points) in;
layout(line_strip, max_vertices = 120) out;

void main()
{
	float dropDistance = distanceFromCam[0];
	int dropsSize = 25;
	for(int i = 1; i < dropsSize; i++){

		float delta = i * 0.1;

		gl_Position = gl_in[0].gl_Position + vec4(0.0, delta, 0.0, 0.0);
		texCoordOut = texCoords[0];
		distanceFromCamera = dropDistance;
		EmitVertex();

		gl_Position = gl_in[0].gl_Position + vec4(0.0, -delta, 0.0, 0.0);
		texCoordOut = texCoords[0];
		distanceFromCamera = dropDistance;
		EmitVertex();

		gl_Position = gl_in[0].gl_Position + vec4(0.1, delta, 0.0, 0.0);
		texCoordOut = texCoords[0];
		distanceFromCamera = dropDistance;
		EmitVertex();

		gl_Position = gl_in[0].gl_Position + vec4(0.1, -delta, 0.0, 0.0);
		texCoordOut = texCoords[0];
		distanceFromCamera = dropDistance;
		EmitVertex();

		EndPrimitive();
	}
}