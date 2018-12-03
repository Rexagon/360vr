#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoords;

uniform mat4 uProjection;
uniform mat4 uRotation;

out vec3 fPosition;

void main()
{
  vec4 position = uProjection * uRotation * vec4(vPosition, 1.0);
  gl_Position = position.xyww;
  
  fPosition = vPosition;
}