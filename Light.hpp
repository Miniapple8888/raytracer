// Termm--Fall 2020

#pragma once

#include <iosfwd>

#include <glm/glm.hpp>

// Represents a simple point light.
struct Light {
  Light();
  glm::vec3 sample_pt_on_surface();
  void illuminate(const glm::vec3 &P, glm::vec3 &lightDir, glm::vec3 &lightIntensity, float &distance) const; 
  glm::vec3 intensity;
  glm::vec3 dir;
  glm::vec3 colour;
  glm::vec3 position;
  float radius;
  double falloff[3];
};

std::ostream& operator<<(std::ostream& out, const Light& l);
