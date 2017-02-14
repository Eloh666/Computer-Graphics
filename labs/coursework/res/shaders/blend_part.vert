#version 440

vec4 blendTextures(in sampler2D tex[2], in sampler2D blendMap, in vec2 tex_coord) {
  // *********************************
  // Sample all Five textures based on weight
  vec4 color1 = texture(tex[0], tex_coord);
  vec4 color2 = texture(tex[1], tex_coord);
  vec4 blend = texture(blendMap, tex_coord);

  return mix(color1, color2, blend.r);

}