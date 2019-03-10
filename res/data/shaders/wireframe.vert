#version 330

layout (location = 0) in vec3 vPosition;

uniform mat4 uCameraViewProjectionMatrix;
uniform mat4 uMeshTransformation;

out vec3 fPosition;

void main()
{
  vec4 globalPosition = uMeshTransformation * vec4(vPosition, 1.0);
  gl_Position = uCameraViewProjectionMatrix * globalPosition;
  fPosition = globalPosition.xyz;
}
