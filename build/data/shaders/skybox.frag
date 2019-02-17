#version 330

#define M_1_2PI 0.1591549430918953357688837633725
#define M_1_PI 0.3183098861837906715377675267450

in vec3 fPosition;

uniform sampler2D uSkyTexture;

out vec3 color;

vec2 sampleSphericalMap(vec3 direction)
{
    vec2 uv = vec2(atan(direction.z, direction.x), asin(direction.y));
    uv *= vec2(M_1_2PI, M_1_PI);
    uv += 0.5;
    return uv;
}

void main()
{
  vec3 direction = normalize(-fPosition);

  float light = 1.0;
  if (direction.y > 0) {
    light = 0.1 + (1 - direction.y) * 0.9;
  }

  vec2 texCoords = sampleSphericalMap(direction);
  color = texture(uSkyTexture, texCoords).xyz * light;
}
