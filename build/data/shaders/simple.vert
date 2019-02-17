#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoords;
layout (location = 2) in vec3 vNormal;

uniform mat4 uCameraViewProjectionMatrix;
uniform mat4 uMeshTransformation;

out vec2 fTexCoords;
out vec3 fNormal;

void main()
{
  gl_Position = uCameraViewProjectionMatrix * uMeshTransformation * vec4(vPosition, 1.0);
  fTexCoords = vTexCoords;
}
