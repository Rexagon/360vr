#version 330

layout (location = 0) in vec3 vPosition;

uniform mat4 uCameraProjectionMatrix;
uniform mat4 uCameraRotationMatrix;

out vec3 fPosition;

void main()
{
  vec4 position = uCameraProjectionMatrix * uCameraRotationMatrix * vec4(vPosition, 1.0);
  gl_Position = position.xyww;
  
  fPosition = vPosition;
}
