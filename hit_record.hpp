#ifndef HIT_RECORD
#define HIT_RECORD

#include <glm/glm.hpp>
#include "Material.hpp"
#include <memory>

class hit_record {
  public:
    glm::vec3 p;
    glm::vec3 normal;
    double t;
    double t1; // end
    bool front_face;
    double u = 0.0;
    double v = 0.0;
    Material* material;
    int i; //index in hitlist.objects

    void set_face_normal(const ray&r, glm::vec3 &outward_normal) {
        front_face = glm::dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

#endif