#version 440

// Incoming position data
layout(location = 0) in vec3 position;

void main() {
  // Transform position into camera space
  gl_Position = vec4(position, 1.0);
}