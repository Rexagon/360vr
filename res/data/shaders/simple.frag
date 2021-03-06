#version 330

in vec3 fPosition;
in vec2 fTexCoords;
in vec3 fNormal;

uniform sampler2D uDiffuseTexture;
uniform vec3 uCameraPosition;
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

  vec3 lightDirection = uCameraPosition - fPosition;
  float light = clamp(dot(normalize(lightDirection), normalize(fNormal)), 0.0, 1.0);
  light = 0.1 + 0.9 * light;

  color = diffuse.rgb * light;
}
