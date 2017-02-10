#version 440

// Point light information
struct point_light {
  vec4 light_colour;
  vec3 position;
  float constant;
  float linear;
  float quadratic;
};

// Material information
struct material {
  vec4 emissive;
  vec4 diffuse_reflection;
  vec4 specular_reflection;
  float shininess;
};

// Point light for the scene
uniform point_light point;
// Material for the object
uniform material mat;
// Eye position
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
  // Get distance between point light and vertex
  float dist = distance(point.position, position);
  // Calculate attenuation factor

  float attenuation = 1.0f / ( point.constant + (dist * point.linear) + (pow(dist, 2) * point.quadratic) );

  // Calculate light colour

  vec4 lightColour = point.light_colour * attenuation;

  // Calculate light dir

  vec3 lightDir = normalize(point.position - position);

  // Calculate k
  float diffuseK = max(dot(normal, lightDir), 0.0);

  // Calculate diffuse
  vec4 diffuse = diffuseK * (mat.diffuse_reflection * lightColour);

  // Calculate view direction
  vec3 viewDir = normalize(eye_pos - position);

  // Calculate half vector between view_dir and light_dir
  vec3 half_vector = normalize(lightDir + viewDir);

  // Calculate k
  float k = pow(max(dot(normal, half_vector), 0.0), mat.shininess);

  // Calculate specular
  vec4 specular = k * (lightColour * mat.specular_reflection);

  // Sample texture
  vec4 texColor = texture(tex, tex_coord);
  // Calculate primary colour component
  vec4 primary = mat.emissive + diffuse;
  // Calculate final colour - remember alpha
  colour = primary * texColor + specular;
  colour[3] = 1.0f;


  // *********************************
}