#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoords;
layout (location = 2) in vec3 vNormal;

uniform mat4 uViewProjection;
uniform mat4 uTransform;

out vec3 fPosition;
out vec2 fTexCoords;
out vec3 fNormal;

void main()
{
  gl_Position = uViewProjection * uTransform * vec4(vPosition, 1.0);

  fPosition = vPosition;
  fTexCoords = vTexCoords;
  fNormal = (transpose(inverse(uTransform)) * vec4(vNormal, 1.0)).xyz;
}