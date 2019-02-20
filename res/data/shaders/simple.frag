#version 330

in vec2 fTexCoords;
in vec3 fNormal;

uniform sampler2D uDiffuseTexture;
uniform vec4 uColor;
uniform ivec2 uTextureFlip;
uniform int uHasTexture;

out vec3 color;

void main()
{
  vec4 diffuse = uColor;
  if (uHasTexture == 1) {
    vec2 texCoords = fTexCoords;

    if (uTextureFlip.x == 1) {
      texCoords.x = 1.0 - texCoords.x;
    }
    if (uTextureFlip.y == 1) {
      texCoords.y = 1.0 - texCoords.y;
    }

    diffuse *= texture(uDiffuseTexture, texCoords);
  }

  color = diffuse.rgb;
}
