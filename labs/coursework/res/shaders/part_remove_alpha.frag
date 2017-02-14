#version 440

vec4 removeAlpha(in vec4 tex_colour, in sampler2D blendMap, in vec2 tex_coord) {
  // *********************************
  // Sample all Five textures based on weight
  vec4 color2 = vec4(0, 0, 0, 0);
  vec4 blend = texture(blendMap, tex_coord);

  return mix(tex_colour, color2, blend.r);

}