// Termm--Fall 2020

#pragma once
#include <iostream>

#include <glm/glm.hpp>

#include "Material.hpp"
#include "hittable.hpp"

class PhongMaterial : public Material {
public:
  PhongMaterial(glm::vec3& kd, glm::vec3& ks, glm::vec3& ka, double shininess);
  bool scatter(const ray& r_in, const hit_record& rec, ray& r_out, glm::vec3& attenuation, std::list<Light*>& lights, const glm::vec3& ambient) override;
  ~PhongMaterial();

private:
  glm::vec3 m_kd; // diffuse
  glm::vec3 m_ks; // specular
  glm::vec3 m_ka; // ambient

  double m_shininess;
};
