
#pragma once
#include "hittable.hpp"

class medium : public hittable { // we asume a spherical medium
public:
    NonhierSphere* sph;
    glm::vec3 center = glm::vec3(0, 0, -4); 
    float radius = 1.0f; 

    medium(NonhierSphere *sph, Material *phase_func, const glm::mat4& trans) : sph(sph) {
        m_material = phase_func;
        tf = trans;
        // TODO: include material + tf
        
    }

    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) override {
        float radius = sph->get_m_radius();
        glm::vec3 center = sph->get_m_pos();
        
        // glm::vec3 new_center = rcenter.at(r.time()); do we want this
        bool intersect = intersect_sphere(r, radius, center, ray_tmin, ray_tmax, rec);
        if (intersect) {
            rec.material = m_material;
            if (glm::abs(rec.t1 - rec.t) < 1e-7f) {
                // same roots, mean only 1 pt of intersection, useless to render
                return false;
            }
            return true;
        }

        return false;
    }

    void updateTf(const glm::mat4 &trans) {
        return;
    }
    void update(int delta_t) {
        return;
    }
    ~medium() {

    }
};