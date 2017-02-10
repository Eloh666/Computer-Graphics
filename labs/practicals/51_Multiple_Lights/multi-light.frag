#version 440

// Point light information
struct point_light {
  vec4 light_colour;
  vec3 position;
  float constant;
  float linear;
  float quadratic;
};

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

// Point lights being used in the scene
uniform point_light points[4];
// Spot lights being used in the scene
uniform spot_light spots[5];
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

// Point light calculation
vec4 calculate_point(in point_light point, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir,
                     in vec4 tex_colour) {
  // *********************************
  // Get distance between point light and vertex
  float dist = distance(point.position, position);

  // Calculate attenuation factor
  float attenuation = 1.0f / ( point.constant + (dist * point.linear) + (pow(dist, 2) * point.quadratic) );

  // Calculate light colour
  vec4 lightColour = point.light_colour * attenuation;

  // Calculate light dir
  vec3 lightDir = normalize(point.position - position);
  
  // Now use standard phong shading but using calculated light colour and direction
  // - note no ambient
  // Calculate k
  float diffuseK = max(dot(normal, lightDir), 0.0);

  // Calculate diffuse
  vec4 diffuse = diffuseK * (mat.diffuse_reflection * lightColour);

  // Calculate half vector between view_dir and light_dir
  vec3 half_vector = normalize(lightDir + view_dir);

  // Calculate k
  float k = pow(max(dot(normal, half_vector), 0.0), mat.shininess);

  // Calculate specular
  vec4 specular = k * (lightColour * mat.specular_reflection);

  // Sample texture
  vec4 texColor = texture(tex, tex_coord);
  // Calculate primary colour component
  vec4 primary = mat.emissive + diffuse;
  // Calculate final colour - remember alpha
  vec4 colour = primary * tex_colour + specular;
  colour[3] = 1.0f;

  // *********************************
  return colour;
}

// Spot light calculation
vec4 calculate_spot(in spot_light spot, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir,
                    in vec4 tex_colour) {
  // *********************************
  // Calculate direction to the light
   vec3 lightDir = normalize(spot.position - position);

  // Calculate distance to light
  float dist = distance(spot.position, position);

  // Calculate attenuation value
  float divider = pow( max(dot(-1 * spot.direction, lightDir), 0), spot.power);
  float attenuation = divider / ( spot.constant + (dist * spot.linear) + (pow(dist, 2) * spot.quadratic) );

  // Calculate spot light intensity

  // Calculate light colour
  vec4 lightColour = spot.light_colour * attenuation;

  // Now use standard phong shading but using calculated light colour and direction
  // - note no ambient

  float diffuseK = max(dot(normal, lightDir), 0.0);

  // Calculate diffuse
  vec4 diffuse = diffuseK * (mat.diffuse_reflection * lightColour);

  // Calculate half vector between view_dir and light_dir
  vec3 half_vector = normalize(lightDir + view_dir);

  // Calculate k
  float k = pow(max(dot(normal, half_vector), 0.0), mat.shininess);

  // Calculate specular
  vec4 specular = k * (lightColour * mat.specular_reflection);

  // Calculate primary colour component
  vec4 primary = mat.emissive + diffuse;
  // Calculate final colour - remember alpha
  vec4 colour = primary * tex_colour + specular;
  colour[3] = 1.0f;
  
  // *********************************
  return colour;
}

void main() {

  colour = vec4(0.0, 0.0, 0.0, 1.0);
  // *********************************
  // Calculate view direction
  vec3 view_dir = normalize(eye_pos - position);

  // Sample texture
  vec4 tex_colour = texture(tex, tex_coord);

  // Sum point lights
  for(int i = 0; i < 4; i++){
	colour += calculate_point(points[i], mat, position, normal, view_dir, tex_colour);
  }

  // Sum spot lights
  for(int i = 0; i < 5; i++){
	colour += calculate_spot(spots[i], mat, position, normal, view_dir, tex_colour);
  }

  // *********************************
}