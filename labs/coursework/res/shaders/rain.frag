#version 440

// Sampler used to get texture colour
uniform sampler2D tex;

// Incoming texture coordinate
layout(location = 0) in vec2 tex_coord;
// Outgoing colour
layout(location = 0) out vec4 out_colour;
layout(location = 1) in float dropDistance;

void main() {

  // *********************************
  // Set out colour to sampled texture colour
  out_colour = texture(tex, tex_coord);
  out_colour.a = 0.45;
  // *********************************
}