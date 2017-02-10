#version 440

// A directional light structure
struct directional_light {
  vec4 ambient_intensity;
  vec4 light_colour;
  vec3 light_dir;
};

// A material structure
struct material {
  vec4 emissive;
  vec4 diffuse_reflection;
  vec4 specular_reflection;
  float shininess;
};

// Directional light for the scene
uniform directional_light light;
// Material of the object
uniform material mat;
// Position of the camera
uniform vec3 eye_pos;
// Texture
uniform sampler2D tex;

// Incoming position
layout(location = 0) in vec3 position;
// Incoming normal
layout(location = 1) in vec3 normal;
// Incoming texture coordinate
layout(location = 2) in vec2 tex_coord;

// Outgoing colour
layout(location = 0) out vec4 colour;

void main() {

  // *********************************
  // Calculate ambient component
  vec4 ambient = light.ambient_intensity * mat.diffuse_reflection;

  // Calculate k
  float diffuseK = max(dot(normal, light.light_dir), 0.0);

  // Calculate diffuse
  vec4 diffuse = diffuseK * (mat.diffuse_reflection * light.light_colour);

  // Calculate view direction
  vec3 viewDir = normalize(eye_pos - position);

  // Calculate half vector between view_dir and light_dir
  vec3 half_vector = normalize(light.light_dir + viewDir);

  // Calculate k
  float k = pow(max(dot(normal, half_vector), 0.0), mat.shininess);

  // Calculate specular
  vec4 specular = k * (light.light_colour * mat.specular_reflection);

  // Sample texture
  vec4 texColor = texture(tex, tex_coord);
  // Calculate primary colour component
  vec4 primary = mat.emissive + ambient + diffuse;
  // Calculate final colour - remember alpha
  colour = primary * texColor + specular;
  colour[3] = 1.0f;
  // *********************************
}