#version 440

// Spot light data
struct spot_light {
  vec4 light_colour;
  vec3 position;
  vec3 direction;
  float constant;
  float linear;
  float quadratic;
  float power;
};

// Material data
struct material {
  vec4 emissive;
  vec4 diffuse_reflection;
  vec4 specular_reflection;
  float shininess;
};

// Spot light being used in the scene
uniform spot_light spot;
// Material of the object being rendered
uniform material mat;
// Position of the eye (camera)
uniform vec3 eye_pos;
// Texture to sample from
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
  // Calculate light dir
  vec3 lightDir = normalize(spot.position - position);

  // Calculate distance to light
  float dist = distance(spot.position, position);

  // Calculate attenuation value
  float divider = pow( max(dot(-1 * spot.direction, lightDir), 0), spot.power);
  float attenuation = divider / ( spot.constant + (dist * spot.linear) + (pow(dist, 2) * spot.quadratic) );
  // Calculate spot light intensity

  // Calculate light colour
  vec4 lightColour = spot.light_colour * attenuation;

  // Calculate view direction
  vec3 viewDir = normalize(eye_pos - position);

  // Now use standard phong shading but using calculated light colour and direction
  // - note no ambient
  // *********************************

  // Calculate k
  float diffuseK = max(dot(normal, lightDir), 0.0);

  // Calculate diffuse
  vec4 diffuse = diffuseK * (mat.diffuse_reflection * lightColour);

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
  colour[3] = 1.0;

  // *********************************
}