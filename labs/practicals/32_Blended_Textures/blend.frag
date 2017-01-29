#version 450

// Main textures
uniform sampler2D tex[2];
// Blend map
uniform sampler2D blend;

// Incoming texture coordinate
layout(location = 0) in vec2 tex_coord;
// Outgoing fragment colour
layout(location = 0) out vec4 colour;

void main() {
  // *********************************
  // Sample the two main textures
  vec4 color1 = texture(tex[0], tex_coord),
  vec4 color2 = texture(tex[1], tex_coord),
  vec4 blendMap = texture(blend, tex_coord)

  colour = mix(color1, color2, blendMap.r)

  // *********************************
}