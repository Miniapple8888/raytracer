#pragma once

#include <vector>
#include <iosfwd>
#include <string>

#include <glm/glm.hpp>

#include "Primitive.hpp"

class Plane : public Primitive {
public:
    Plane();
    Plane(double *p0, double *p1, double *p2);
    // to define parametric equation of a plane
    glm::vec3 m_p0;
    glm::vec3 m_p1;
    glm::vec3 m_p2;
    glm::vec3 n; // normal (A,B,C)
    float D;
    ~Plane();
};