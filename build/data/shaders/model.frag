#version 330

in vec2 fTexCoords;
in vec3 fNormal;

uniform vec3 uCameraPosition;
uniform sampler2D uDiffuseTexture;

out vec3 color;

void main()
{
  float len = length(uCameraPosition);
  color = texture(uDiffuseTexture, fTexCoords).xyz;
}