// Termm--Fall 2020
#include <glm/glm.hpp>
#include "Primitive.hpp"

Primitive::~Primitive()
{
}


double Cube::get_size() {
    return m_size;
}

glm::vec3 Cube::get_pos() {
    return m_pos;
}


NonhierSphere::~NonhierSphere()
{
}

double NonhierSphere::get_m_radius() {
    return m_radius;
}
glm::vec3 NonhierSphere::get_m_pos() {
    return m_pos;
}

void NonhierSphere::set_m_pos(glm::vec3& p) {
    m_pos = p;
}

NonhierBox::NonhierBox(const glm::vec3& pos, double size)
    : m_pos(pos), m_size(size) {
        m_rot = glm::vec3(0.0f, 0.0f, 0.0f);
}

double NonhierBox::get_size() {
    return m_size;
}
glm::vec3 NonhierBox::get_pos() {
    return m_pos;
}

void NonhierBox::set_pos(glm::vec3& pos) {
    m_pos = pos;
}


NonhierBox::~NonhierBox()
{
}
