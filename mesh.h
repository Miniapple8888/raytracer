#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include "Material.hpp"
#include "Mesh.hpp"
#include "hittable.hpp"
#include <cfloat>

// Uncomment this if want to see bounding volumes for meshes (spherical)
// #define RENDER_BOUNDING_VOLUMES

glm::vec3 compute_normal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
    glm::vec3 v1 = p2 - p1;
    glm::vec3 v2 = p3 - p1;
    return glm::normalize(glm::cross(v1, v2));
}

class mesh : public hittable {
public:
    Mesh *me;
    /// bounding volume
    float radius;
    glm::vec3 center;
    mesh() {}
    mesh(Mesh* m, Material* mat, const glm::mat4& trans) : me(m) {
        m_material = mat;
        tf = trans;
        transform_mesh();
    }

    void transform_mesh() {
        // transform mesh
        for (size_t i = 0; i< me->m_vertices.size(); i++) {
            glm::vec4 homog_coord = glm::vec4(me->m_vertices[i], 1.0f);
            glm::vec4 new_coord = tf * homog_coord;
            me->m_vertices[i] = glm::vec3(new_coord.x / new_coord.w,
                                            new_coord.y / new_coord.w,
                                            new_coord.z / new_coord.w);
        }
    }

    void compute_sphere() {
        // bounding volume sphere
        // loop through each vertex, find minmax,x,y,z
        //if (vertices.empty()) return {glm::vec3(0), 0.0f};

        // Step 1: Compute AABB
        glm::vec3 min = me->m_vertices[0];
        glm::vec3 max = me->m_vertices[0];
        for (const auto& v : me->m_vertices) {
            min = glm::min(min, v);
            max = glm::max(max, v);
        }

        // Step 2: Compute sphere center (center of AABB)
        center = (min + max) * 0.5f;

        // Step 3: Compute radius (max distance from center to any vertex)
        radius = 0.0f;
        for (const auto& v : me->m_vertices) {
            float dist = glm::distance(center, v);
            radius = std::max(radius, dist);
        }
    }

    void update(int delta_t) {
        // acceleration
        // glm::vec3 pos = b->get_pos();
        // update(pos, delta_t);
        // sph->set_pos(pos);
        // TODO
        std::cout<<"update nothimng"<<std::endl;
    }

    void updateTf(const glm::mat4 &trans) override {
        tf = trans;
    }

    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) override {
        glm::vec3 orig = r.origin();
        glm::vec3 dir = r.direction();
        float tNear = FLT_MAX;
        int triIndex;

        compute_sphere();

        if (!intersect_sphere(r, radius, center, ray_tmin, ray_tmax, rec)) return false; // check if intersect with bounding volume

        #ifdef RENDER_BOUNDING_VOLUMES
            rec.material = m_material; // render bounding volumes instead of actual mesh
            return true;
        #endif
        // if (material==nullptr) std::cout<<"unfort"<<std::endl;
        // std::cout<<"num faces:"<<std::to_string(me->m_faces.size())<<std::endl;
        bool intersect = false; // intersected or not
        // iterate through all triangles in the mesh
        for (int i = 0; i < me->m_faces.size(); ++i) { 
            Triangle tri = me->m_faces[i];
            const glm::vec3 &v1 = me->m_vertices[tri.v1]; 
            const glm::vec3 &v2 = me->m_vertices[tri.v2]; 
            const glm::vec3 &v3 = me->m_vertices[tri.v3]; 
            float t, u, v; 
            // std::cout<<"direction:"<<glm::to_string(dir)<<std::endl;
            // std::cout<<"v1:"<<glm::to_string(v1)<<"v2:"<<glm::to_string(v2)<<"v3:"<<glm::to_string(v3)<<std::endl;
            if (intersectRayTriangle(orig, dir, v1, v2, v3, t, u, v) && t < tNear) { 
                // std::cout <<"intersected?"<<std::to_string(intersect)<<std::endl;
                tNear = t; 
                rec.t = tNear;
                rec.p = r.at(rec.t);
                rec.normal = compute_normal(v1, v2, v3);

                // std::cout<<"t"<<std::to_string(rec.t)<<std::endl;
                // std::cout<<"p"<<glm::to_string(rec.p)<<std::endl;
                rec.material = m_material;
                triIndex = i; 
                intersect |= true; 
                // std::cout<<"v1:"<<glm::to_string(v1)<<"v2:"<<glm::to_string(v2)<<"v3:"<<glm::to_string(v3)<<std::endl;
            }
        }
        
        return intersect;
    }

    bool intersectRayTriangle(const glm::vec3& O, const glm::vec3& D,
                          const glm::vec3& V0, const glm::vec3& V1, const glm::vec3& V2,
                          float& t, float& u, float& v) const {
        const float EPSILON = 1e-8; // to account for floating point error
        // Moller Trumbore algorithm
        
        glm::vec3 E1 = V1 - V0;
        glm::vec3 E2 = V2 - V0;
        glm::vec3 P = glm::cross(D, E2);
        float det = glm::dot(E1, P);
        if (fabs(det) < EPSILON) return false; // ray parallel to triangle

        float invDet = 1.0 / det;
        glm::vec3 T = O - V0;
        u = glm::dot(T, P) * invDet;
        if (u < 0 || u > 1) return false; // intersection outside of triangle

        glm::vec3 Q = glm::cross(T, E1);
        v = glm::dot(D, Q) * invDet;
        if (v < 0 || u + v > 1) return false; // intersection outside triangle

        t = glm::dot(E2, Q) * invDet;
        return t > EPSILON; // Valid intersection
    }

};

#endif
