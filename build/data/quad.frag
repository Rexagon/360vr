#version 330

uniform sampler2D diffuseTexture;

in vec2 fTexCoords;

out vec3 color;

void main()
{
  vec2 texCoords = vec2(fTexCoords.x, 1.0 - fTexCoords.y);
  color = texture(diffuseTexture, texCoords).xyz;
}