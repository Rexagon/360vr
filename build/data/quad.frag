#version 330

uniform sampler2D diffuseTexture;

in vec2 fTexCoords;

out vec3 color;

void main()
{
  color = texture(diffuseTexture, fTexCoords).xyz;
}