#ifndef PLANE_H
#define PLANE_H

#include <glm/glm.hpp>
#include "Plane.hpp"
#include "hittable.hpp"

class plane : public hittable {
public:
    Plane *p;
    plane() {
    }
    plane(Plane *pl, Material* mat, const glm::mat4& trans) : p(pl) {
        m_material =mat;
        tf = trans; // apply transform
    }

    void update(int delta_t) {
        // acceleration
        //TODO
        //  void update(int delta_t) {
        // // acceleration
        // glm::vec3 pos = b->get_pos();
        // update(pos, delta_t);
        // sph->set_pos(pos);
        std::cout<<"update nothimng"<<std::endl;
    }

    void updateTf(const glm::mat4 &trans) override {
        tf = trans;
    }


    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) override {
        // find intersection with plane
        float denom = glm::dot(p->n, r.direction());
        if (denom == 0.0f) return false; // avoid division by zero
        float t = (p->D - glm::dot(p->n, r.origin())) / denom;
        
        // do some checks with where t intersects
        if (t > 0) {
            rec.t = t;
            rec.normal = p->n;
            rec.set_face_normal(r, rec.normal);
            rec.p = r.at(rec.t);
            rec.material = m_material;
            return true;
        }
        return false;
    }
};

#endif