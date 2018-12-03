#version 330

in vec2 fTexCoords;
in vec3 fNormal;

uniform sampler2D uDiffuseTexture;

out vec3 color;

void main()
{
  color = texture(uDiffuseTexture, fTexCoords).xyz;
}