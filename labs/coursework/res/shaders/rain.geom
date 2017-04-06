#version 440

layout (location = 0) out vec2 tex_coord;
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

void main()
{
    vec4 position = gl_in[0].gl_Position;
    // Grass patch bottom left vertex
	  vec2 va = position.xy + vec2(3.00, -10);
    gl_Position = position + vec4(va, position.zw);
    tex_coord = vec2(0.0, 0.0);
    EmitVertex();

    // Vertex 2 is bottom right
	  vec2 br = position.xy + vec2(3.05, -10);
    gl_Position = position + vec4(br, position.zw);
    tex_coord = vec2(1.0, 0.0);
    EmitVertex();

    // Vertex 3 is top left
	  vec2 tl = position.xy + vec2(-0.05, 10);
    gl_Position = position + vec4(tl, position.zw);
    tex_coord = vec2(0.0, 1.0);
    EmitVertex();

    // Vertex 4 it top right
	  vec2 tr = position.xy + vec2(0.05, 10);
    gl_Position = position + vec4(tr, position.zw);
    tex_coord = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();
}


