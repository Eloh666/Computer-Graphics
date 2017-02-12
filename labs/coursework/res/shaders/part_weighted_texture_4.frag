#version 440

vec4 weighted_texture(in sampler2D tex[4], in vec2 tex_coord, in vec4 weights) {
  vec4 tex_colour = vec4(0, 0, 0, 1);
  // *********************************
  // Sample all Five textures based on weight
  for(int i = 0; i < tex.length(); i++){
	tex_colour += texture(tex[i], tex_coord) * weights[i];
  }
  // *********************************
  return tex_colour;
}