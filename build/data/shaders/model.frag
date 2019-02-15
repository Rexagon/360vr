#version 330

in vec2 fTexCoords;
in vec3 fNormal;

uniform vec3 uCameraPosition;
uniform sampler2D uDiffuseTexture;
uniform int uFlipTexture;

out vec3 color;

void main()
{
  float len = length(uCameraPosition);

  vec2 texCoords = fTexCoords;
  if (uFlipTexture == 1) {
    texCoords.x = 1.0 - texCoords.x;
  }  
  color = texture(uDiffuseTexture, texCoords).xyz;
}