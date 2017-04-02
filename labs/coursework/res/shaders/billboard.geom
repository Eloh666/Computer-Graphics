#version 440

// The projection transformation
uniform mat4 MVP;

// Point size for the billboards
uniform float point_size;

// Incoming data
layout(points) in;
// Outgoing data
layout(triangle_strip, max_vertices = 4) out;

// Outgoing texture coordinate
layout(location = 0) out vec2 tex_coord;


mat4 getRotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

vec3 vLocalSeed;

void main() {
  // Incoming position
  vec4 pointPosition = gl_in[0].gl_Position;

  float PIover180 = 3.1415/180.0;
  vec3 vBaseDir[3] = vec3[](
		vec3(1.0, 0.0, 0.0),
		vec3(float(cos(45.0*PIover180)), 0.0f, float(sin(45.0*PIover180))),
		vec3(float(cos(-45.0*PIover180)), 0.0f, float(sin(-45.0*PIover180)))
	);

	for(int i = 0; i < 3; i++){

	 vec3 vBaseDirRotated = (getRotationMatrix(vec3(0, 1, 0), sin(i*0.7)*0.1)*vec4(vBaseDir[i], 1.0)).xyz;

	  // Vertex 1 is bottom left
	  vec2 va = pointPosition.xy + vec2(-10.5, -10.5) * point_size;
	  gl_Position = MVP * vec4(va, pointPosition.zw);
	  tex_coord = vec2(0.0, 0.0);
	  EmitVertex();

	  // Vertex 2 is bottom right
	  vec2 br = pointPosition.xy + vec2(10.5, -10.5) * point_size;
	  gl_Position = MVP * vec4(br, pointPosition.zw);
	  tex_coord = vec2(1.0, 0.0);
	  EmitVertex();

	  // Vertex 3 is top left
	  vec2 tl = pointPosition.xy + vec2(-10.5, 10.5) * point_size;
	  gl_Position = MVP * vec4(tl, pointPosition.zw);
	  tex_coord = vec2(0.0, 1.0);
	  EmitVertex();
  
	  // Vertex 4 it top right
	  vec2 tr = pointPosition.xy + vec2(10.5, 10.5) * point_size;
	  gl_Position = MVP * vec4(tr, pointPosition.zw);
	  tex_coord = vec2(1.0, 1.0);
	  EmitVertex();

	  // End Primitive
	  EndPrimitive();
	}
}