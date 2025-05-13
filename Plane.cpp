#include <glm/glm.hpp>
#include "Plane.hpp"

Plane::Plane() {

}

Plane::Plane(double *p0, double *p1, double *p2) {
    m_p0 = glm::vec3(p0[0],p0[1],p0[2]);
    m_p1 = glm::vec3(p1[0],p1[1],p1[2]);
    m_p2 = glm::vec3(p2[0],p2[1],p2[2]);
    glm::vec3 v1 = m_p1 - m_p0;
    glm::vec3 v2 = m_p2 - m_p1;
    // consider colinear problem where all 3 points lie on the same line
    n = glm::cross(v1,v2);
    n = glm::normalize(n);
    D = glm::dot(n, m_p0);
}

Plane::~Plane() {
    
}