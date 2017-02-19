#version 440 core
// Outgoing colour
layout(location = 0) out vec4 colour;

//uniform sampler2D texture_diffuse1;

void main()
{
  //  color = texture(texture_diffuse1, TexCoords);
  colour = vec4(1.0, 0.0, 0.0, 1.0);
}