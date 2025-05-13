#ifndef CUBE_H
#define CUBE_H

#include <glm/glm.hpp>
#include "Material.hpp"
#include "Primitive.hpp"
#include "hittable.hpp"
#include "Mesh.hpp"
#include "mesh.h"


class cube : public mesh {
public:
    NonhierBox *b;
    Material *material;

    cube(NonhierBox *b, Material* mat, const glm::mat4& trans) : b(b) {
        m_material = mat; 
        tf = trans;
        // convert nonhier box to mesh
        float m_size = b->get_size();
        glm::vec3 m_pos = b->get_pos();

        glm::vec3 A = m_pos + glm::vec3(m_size, m_size, m_size);
        glm::vec3 B = m_pos + glm::vec3(m_size, -m_size, m_size);
        glm::vec3 C = m_pos + glm::vec3(-m_size, -m_size, m_size);
        glm::vec3 D = m_pos + glm::vec3(-m_size, m_size, m_size);
        glm::vec3 E = m_pos + glm::vec3(-m_size, m_size, -m_size);
        glm::vec3 F = m_pos + glm::vec3(m_size, m_size, -m_size);
        glm::vec3 G = m_pos + glm::vec3(m_size, -m_size, -m_size);
        glm::vec3 H = m_pos + glm::vec3(-m_size, -m_size, -m_size);
        me = new Mesh();

        me->m_vertices.emplace_back(A); //0
        me->m_vertices.emplace_back(B); //1
        me->m_vertices.emplace_back(C); //2
        me->m_vertices.emplace_back(D); //3
        me->m_vertices.emplace_back(E); //4
        me->m_vertices.emplace_back(F); //5
        me->m_vertices.emplace_back(G); //6
        me->m_vertices.emplace_back(H); //7

        me->m_faces.emplace_back(Triangle(2,7,4));
        me->m_faces.emplace_back(Triangle(2,3,4));
        // normals[0] = computeNormal(C, H, E); // left
        me->m_faces.emplace_back(Triangle(5,6,0));
        me->m_faces.emplace_back(Triangle(0,6,1));
        // normals[1] = computeNormal(F, G, A); // right
        me->m_faces.emplace_back(Triangle(2,7,6));
        me->m_faces.emplace_back(Triangle(1,2,6));
        // normals[2] = computeNormal(C, H, G); // bottom
        me->m_faces.emplace_back(Triangle(4,5,3));
        me->m_faces.emplace_back(Triangle(0,5,3));
        // normals[3] = computeNormal(E, F, D); // top
        me->m_faces.emplace_back(Triangle(0,1,3));
        me->m_faces.emplace_back(Triangle(1,2,3));
        // normals[4] = computeNormal(A, B, D); // front
        me->m_faces.emplace_back(Triangle(4,5,7));
        me->m_faces.emplace_back(Triangle(5,6,7));
        // normals[5] = computeNormal(E, F, H); // back

        transform_mesh();
    }

    void update(int delta_t) {
        // acceleration
        // glm::vec3 pos = b->get_pos();
        // update(pos, delta_t);
        // b->set_pos(pos);
        std::cout<<"to be updated"<<std::endl;
    }

    void updateTf(const glm::mat4 &trans) override {
        tf = trans;
    }

    // we don't override the hit function, handled by mesh
    ~cube() {
        std::cout <<"deallocate me"<<std::endl;
        delete me;
        me = nullptr;
    }
};

#endif