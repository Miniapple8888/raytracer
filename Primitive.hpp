// Termm--Fall 2020

#pragma once

#include <glm/glm.hpp>
#include "Rigidbody.hpp"

class Primitive {
public:
  virtual ~Primitive();
};

class NonhierSphere : public Primitive {
  public:
    RigidBody *rb = nullptr;
    NonhierSphere() {}
    NonhierSphere(const glm::vec3& pos, double radius)
      : m_pos(pos), m_radius(radius)
    {
    }
    virtual ~NonhierSphere();
    glm::vec3 get_m_pos();
    double get_m_radius();
    void set_m_pos(glm::vec3& p);
  
  protected:
    glm::vec3 m_pos;
    double m_radius;
  };

class MediumSphere : public NonhierSphere {
public:
  MediumSphere() {}
  MediumSphere(const glm::vec3& pos, double radius)
      : NonhierSphere(pos, radius)
    {}
  virtual ~MediumSphere() {};
};
  
class NonhierBox : public Primitive {
  public:
    NonhierBox() {}
    NonhierBox(const glm::vec3& pos, double size);
    double get_size();
    glm::vec3 get_pos();
    glm::vec3 get_rot();
    void set_pos(glm::vec3& pos);
    virtual ~NonhierBox();
  
  private:
    glm::vec3 m_pos;
    double m_size;
    glm::vec3 m_rot;
  };
  
class Sphere : public NonhierSphere {
public:
  Sphere() {
    m_radius = 1;
    m_pos = glm::vec3(0.0f,0.0f,0.0f);
  }
  ~Sphere() {};
private:
  glm::vec3 m_pos;
  double m_radius;
};

class Cube : public NonhierBox {
public:
  Cube() {
    m_size = 1;
    m_pos = glm::vec3(0.0f,0.0f,0.0f);
  }
  glm::vec3 get_pos();
  double get_size();
  ~Cube() {};

private:
  glm::vec3 m_pos;
  double m_size;
};

