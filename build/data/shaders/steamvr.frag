#version 330

#define M_1_2PI 0.1591549430918953357688837633725
#define M_1_PI 0.3183098861837906715377675267450

in vec3 fPosition;
in vec2 fTexCoords;
in vec3 fNormal;

uniform sampler2D uDiffuseTexture;
uniform sampler2D uSkyboxTexture;
uniform vec3 uCameraPosition;

vec2 sampleSphericalMap(vec3 direction)
{
  direction *= -1.0;
  vec2 uv = vec2(atan(direction.z, direction.x), asin(direction.y));
  uv *= vec2(M_1_2PI, M_1_PI);
  uv += 0.5;
  return uv;
}

out vec3 color;

void main()
{
  float ratio = 1.0 / 1.3;
  vec3 I = normalize(fPosition - uCameraPosition);
  vec3 R = refract(I, normalize(fNormal), ratio);

  color = texture(uSkyboxTexture, sampleSphericalMap(R)).xyz;
}