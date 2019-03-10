#version 330

in vec3 fPosition;

uniform vec3 uCameraPosition;
uniform vec4 uColor;

out vec4 color;

void main()
{
  float r = 2.0; // light radius
  vec3 L = vec3(uCameraPosition.x, 0.0, uCameraPosition.z) - fPosition;
  float distance = length(L);
  float d = max(distance - r, 0);
  L /= distance;
    
  // calculate basic attenuation
  float denom = d/r + 1;
  float attenuation = 1 / (denom*denom);

  float cutoff = 0.05;
    
  // scale and bias attenuation such that:
  //   attenuation == 0 at extent of max influence
  //   attenuation == 1 when d == 0
  attenuation = (attenuation - cutoff) / (1 - cutoff);
  attenuation = max(attenuation, 0);

  color = vec4(uColor.xyz, attenuation);
}
