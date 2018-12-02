#version 330

#define M_1_2PI 0.1591549430918953357688837633725
#define M_1_PI 0.3183098861837906715377675267450

uniform sampler2D diffuseTexture;

in vec3 fPosition;

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
  vec2 texCoords = sampleSphericalMap(normalize(-fPosition));

  color = texture(diffuseTexture, texCoords).xyz;
}