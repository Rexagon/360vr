#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoords;
layout (location = 2) in vec3 vNormal;

uniform mat4 uCameraViewProjectionMatrix;
uniform mat4 uMeshTransformation;

out vec3 fPosition;
out vec2 fTexCoords;
out vec3 fNormal;

void main()
{
  vec4 globalPosition = uMeshTransformation * vec4(vPosition, 1.0);
  gl_Position = uCameraViewProjectionMatrix * globalPosition;
  fPosition = globalPosition.xyz;
  fTexCoords = vTexCoords;
  fNormal = (transpose(inverse(uMeshTransformation)) * vec4(vNormal, 1.0)).xyz;
}
