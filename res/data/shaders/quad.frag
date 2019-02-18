#version 330

uniform sampler2D uLeftEyeTexture;
uniform sampler2D uRightEyeTexture;

in vec2 fTexCoords;

out vec3 color;

void main()
{
  if (fTexCoords.x < 0.5) {
    color = texture(uLeftEyeTexture, fTexCoords).xyz;
  }
  else {
    color = texture(uRightEyeTexture, fTexCoords).xyz;
  }
}