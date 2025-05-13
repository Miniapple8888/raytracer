// Termm--Fall 2020
#include <iostream>

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "Light.hpp"

Light::Light()
  : colour(0.0, 0.0, 0.0),
    position(0.0, 0.0, 0.0)
{
  intensity = glm::vec3(1.0f,1.0f,1.0f);
  dir = glm::vec3(0.0f,-1.0f,1.0f); //directional light pointing below
  radius = 5.0f;
  falloff[0] = 1.0;
  falloff[1] = 0.0;
  falloff[2] = 0.0;
}

glm::vec3 Light::sample_pt_on_surface() {
  // sample pt on spherical light
  float theta = glm::linearRand(0.0f, 2 * (float)M_PI);  // Azimuthal angle
  float phi = glm::linearRand(0.0f, (float)M_PI);  // Polar angle
  
  float x = position.x + radius * glm::sin(phi) * glm::cos(theta);
  float y = position.y + radius * glm::sin(phi) * glm::sin(theta);
  float z = position.z + radius * glm::cos(phi);
  
  return glm::vec3(x, y, z);
}

void Light::illuminate(const glm::vec3 &P, glm::vec3 &lightDir, glm::vec3 &lightIntensity, float &distance) const {
    lightDir = (P - position);
    float r2 = glm::dot(lightDir, lightDir);
    distance = sqrt(r2);
    lightDir.x /= distance, lightDir.y /= distance, lightDir.z /= distance;
    // avoid division by 0
    lightIntensity = colour * intensity / (4 * M_PI * r2);
}

std::ostream& operator<<(std::ostream& out, const Light& l)
{
  out << "L[" << glm::to_string(l.colour) 
  	  << ", " << glm::to_string(l.position) << ", ";
  for (int i = 0; i < 3; i++) {
    if (i > 0) out << ", ";
    out << l.falloff[i];
  }
  out << "]";
  return out;
}
