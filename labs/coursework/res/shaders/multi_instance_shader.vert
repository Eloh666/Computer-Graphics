#version 440

// The camera projection
uniform mat4 projection;
// The camera view
uniform mat4 view;
// The normal matrix
uniform mat3 N;
// The light transformation matrix
uniform mat4 lightMVPPartial;

// Incoming position
layout(location = 0) in vec3 position;
// Incoming normal
layout(location = 2) in vec3 normal;
// Incoming binormal
layout(location = 3) in vec3 binormal;
// Incoming tangent
layout(location = 4) in vec3 tangent;
// Incoming texture coordinate
layout(location = 10) in vec2 tex_coord_in;
// Incoming instance meatrix M
layout (location = 5) in mat4 instanceMatrix;

// Outgoing vertex position
layout(location = 0) out vec3 vertex_position;
// Outgoing normal
layout(location = 1) out vec3 transformed_normal;
// Outgoing texture coordinate
layout(location = 2) out vec2 tex_coord_out;
// Outgoing tangent
layout(location = 3) out vec3 tangent_out;
// Outgoing binormal
layout(location = 4) out vec3 binormal_out;
// Outgoing position in light space
layout (location = 9) out vec4 vertex_light;

void main() {
  // Transform position into screen space
  gl_Position = projection * view * instanceMatrix * vec4(position, 1.0f);
  // Transform position into world space
  vertex_position = (instanceMatrix * vec4(position, 1.0)).xyz;
  // Pass through texture coordinate
  tex_coord_out = tex_coord_in;
  // Transform normal
  transformed_normal = N * normal;
  // Transform tangent
   tangent_out = N * tangent;
  // Transform binormal
   binormal_out = N * binormal;
  
   // Transform position into light space
	vertex_light = lightMVPPartial * instanceMatrix * vec4(position, 1.0);

}