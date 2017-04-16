#version 440
uniform mat4 MVP;
layout (location = 0) out vec2 tex_coord;
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

void renderVertex(vec4 position, vec3 vertex){
		vec3 pos = position.xyz + vertex;
		gl_Position = MVP * (position + vec4(pos, position.w));
		tex_coord = vec2(1.0, 0.0);
		EmitVertex();
}

void main()
{
	
	vec3 vertices[4] = vec3[](
		vec3(-6.1, -10.0, 0.0),
		vec3(0.1, 10.0, 0.0),
		vec3(0, 10.0, 0.0),
		vec3(-6.0, -10.0, 0.1)
	);

	int indices[4] = int[](
			// Front
			3, 0, 2, 1
	);

    for(int i = 0; i < 4; i++){
		vec4 position = gl_in[0].gl_Position;
		int index = indices[i];
		renderVertex(position, vertices[index]);
	}

    EndPrimitive();
}