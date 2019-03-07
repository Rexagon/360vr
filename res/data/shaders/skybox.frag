#version 330

#define M_1_DIV_2PI 0.1591549430918953357688837633725
#define M_1_DIV_PI 0.3183098861837906715377675267450

in vec3 fPosition;

uniform sampler2D uSkyTexture;
uniform sampler2D uNextSkyTexture;
uniform float uTransition;

uniform int uHasTexture;
uniform int uHasNextTexture;

out vec3 color;

vec2 sampleSphericalMap(vec3 direction)
{
    vec2 uv = vec2(atan(direction.z, direction.x), asin(direction.y));
    uv *= vec2(M_1_DIV_2PI, M_1_DIV_PI);
    uv += 0.5;
    return uv;
}

vec3 getSkyBoxPixel(sampler2D diffuse, vec3 direction) 
{
  vec2 texCoords = sampleSphericalMap(direction);
  return texture(diffuse, texCoords).xyz;
}

void main()
{
  vec3 direction = normalize(-fPosition);

  // Sample current texture
  vec3 firstColor = vec3(0.2);
  if (uHasTexture == 1) {
    firstColor = getSkyBoxPixel(uSkyTexture, direction);
  }

  // Sample next texture
  vec3 secondColor = vec3(0.2);
  if (uHasNextTexture == 1) {
    secondColor = getSkyBoxPixel(uNextSkyTexture, direction);
  }

  // Lerp
  color = mix(firstColor, secondColor, uTransition);

  // Apply basic lighting
  float light = 1.0;
  if (direction.y > 0) {
    light = 0.1 + (1 - direction.y) * 0.9;
  }

  color *= light;
}
