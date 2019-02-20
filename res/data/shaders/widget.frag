#version 330

in vec2 fTexCoords;

uniform sampler2D uTexure;
uniform vec4 uColor;
uniform int uHasTexture;

out vec3 color;

void main()
{
  color = uColor.rgb;
}
