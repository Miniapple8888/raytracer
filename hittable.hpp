#ifndef HITTABLE_H
#define HITTABLE_H

#include <iostream>
#include <glm/glm.hpp>
#include "Material.hpp"
#include "ray.h"
#include "hit_record.hpp"
#include "polyroots.hpp"

class hittable {
  public:
    glm::mat4 tf = glm::mat4(1.0f);
    Material *m_material;
    
    virtual ~hittable() = default;

    // bool intersect_sphere(const ray&r, const float radius, const glm::vec3& center, double ray_tmin, double ray_tmax, hit_record& rec);

    bool intersect_sphere(const ray&r, const float radius, const glm::vec3& center, double ray_tmin, double ray_tmax, hit_record& rec) {
        glm::vec3 CQ = center - r.origin();
        auto a = glm::dot(r.direction(), r.direction());
        auto b = -2.0 * glm::dot(r.direction(), CQ);
        auto c = glm::dot(CQ,CQ) - radius*radius;

        double roots[2];
        size_t num_roots = quadraticRoots(a,b,c,roots);
        if (num_roots == 0){
            return false;
        }
        
        if (num_roots == 1) {
            if (roots[0] >= ray_tmin && roots[0] <= ray_tmax) {
                
                rec.t = roots[0];
            } else {
                return false;
            }
        }

        // select smallest positive root
        if (roots[0] > ray_tmin && roots[0] <= ray_tmax && roots[1] > ray_tmin && roots[1] <= ray_tmax) {
            rec.t = std::min(roots[0], roots[1]);
            rec.t1 = std::max(roots[0], roots[1]); 
        } else if (roots[0] >= ray_tmin && roots[0] <= ray_tmax) {
            rec.t = roots[0];
            rec.t1 = roots[0]; //same thing
        } else if (roots[1] >= ray_tmin && roots[1] <= ray_tmax) {
            rec.t = roots[1];
            rec.t1 = roots[1]; //same thing
        } else {
            return false;
        }
        rec.p = r.at(rec.t);
        rec.normal = glm::normalize((rec.p - center) / static_cast<float>(radius));

        glm::vec3 outward_normal = glm::normalize((rec.p - center) / static_cast<float>(radius));
        rec.set_face_normal(r, outward_normal);

        return true;
    }

    virtual void updateTf(const glm::mat4 &trans) = 0;
    virtual void update(int delta_t) = 0;

    glm::mat4 getTf() const {
        return tf;
    }

    virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) = 0;
    
};

#endif