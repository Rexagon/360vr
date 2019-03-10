#version 330

in vec3 fPosition;

uniform vec3 uCameraPosition;
uniform vec4 uColor;

out vec4 color;

void main()
{
  float d = distance(vec3(uCameraPosition.x, 0.0, uCameraPosition.z), fPosition);

  float light = 1.0 / pow(d + 1.0, 2);

  color = vec4(uColor.xyz, light);
}
