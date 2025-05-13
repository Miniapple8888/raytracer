// Termm--Fall 2020

#pragma once
#include <list>
#include "Light.hpp"
#include "ray.h"
#include "Texture.hpp"
#include "Rigidbody.hpp"

class hit_record;// forward declaration to prevent circular dependency

class Material {
public:
  Texture *texture = nullptr; // nullptr by default
  // RigidBody *rb = nullptr; // does not belong here (temporal)
  virtual bool scatter(const ray& r_in, const hit_record& rec, 
    ray& r_out, glm::vec3& attenuation, std::list<Light*>& lights, const glm::vec3& ambient) = 0; 
  ~Material() = default;
protected:
  Material();
};
