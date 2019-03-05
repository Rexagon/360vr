#version 330

in vec2 fTexCoords;

uniform sampler2D uTexture;
uniform vec4 uColor;
uniform int uHasTexture;

out vec4 color;

void main()
{
  color = uColor;
  if (uHasTexture == 1) {
    color *= texture(uTexture, fTexCoords);
  }
}
