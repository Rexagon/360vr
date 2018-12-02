#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoords;

uniform mat4 viewProjection;

out vec3 fPosition;

void main()
{
  gl_Position = viewProjection * vec4(vPosition, 1.0);
  fPosition = vPosition;
}