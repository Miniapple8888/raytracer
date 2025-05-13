#ifndef SPHERE_H
#define SPHERE_H

#include <memory>
#include "Material.hpp"
#include "Primitive.hpp"
#include "hittable.hpp"
#include "Rigidbody.hpp"

class sphere : public hittable {
  public:
    NonhierSphere *sph;
    RigidBody *rb = nullptr;
    sphere(NonhierSphere *sph, Material* mat, const glm::mat4& trans) : sph(sph)  {
      m_material = mat;
      updateTf(trans);

      rcenter = ray(sph->get_m_pos(), glm::vec3(0,0,0)); // static center
    }

    sphere(NonhierSphere *sph, Material* mat, const glm::mat4& trans, RigidBody *rb) : sph(sph)  {
      m_material = mat;
      updateTf(trans);

      rcenter = ray(sph->get_m_pos(), glm::vec3(0,0,0)); // static center
    }

    void motionBlurUpdate(glm::vec3& new_center) {
      center = sph->get_m_pos();
      center2 = new_center;
      sph->set_m_pos(new_center);
      rcenter = ray(center, center2-center);
    }

    // part that should be in rigidbody
    void update(int delta_t) {
        // acceleration
        if (rb != nullptr) {
          glm::vec3 pos = sph->get_m_pos(); //get center of sphere
          rb->update(pos, delta_t);
          sph->set_m_pos(pos);
        }
        return;
    }

    void updateTf(const glm::mat4& trans) override {
      // handle scale
      tf *= trans;
      glm::vec3 translation = glm::vec3(trans[3][0],trans[3][1],trans[3][2]);
      glm::vec3 new_pos = sph->get_m_pos() + translation;
      sph->set_m_pos(new_pos);
    }

    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) override {
        float radius = sph->get_m_radius();
        glm::vec3 center = sph->get_m_pos();
        
        glm::vec3 new_center = rcenter.at(r.time());
        // glm::vec3 new_center = center;
        bool intersect = intersect_sphere(r, radius, new_center, ray_tmin, ray_tmax, rec);
        if (intersect) {
          rec.material = m_material;

          // for texture mapping
          glm::vec3 norm_pt = glm::normalize(rec.p - center);
          rec.u = 0.5 + glm::atan(norm_pt.z,norm_pt.x)/(2*glm::pi<float>());
          rec.v = 0.5 + glm::asin(norm_pt.y)/glm::pi<float>();
          // clamp u,v to (0,1)
          rec.u = glm::clamp(rec.u, 0., 1.);
          rec.v = glm::clamp(rec.v, 0., 1.);
        }

        return intersect;
    }

  private:
    ray rcenter;
    glm::vec3 center;
    glm::vec3 center2;
    double radius;
};

#endif
