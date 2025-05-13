#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

class ray {
  public:
    ray() {}

    ray(const glm::vec3& origin, const glm::vec3& direction, double time) : orig(origin), dir(direction), tm(time) {}
    ray(const glm::vec3& origin, const glm::vec3& direction) : orig(origin), dir(direction), tm(0.0) {}

    const glm::vec3& origin() const  { return orig; }
    const glm::vec3& direction() const { return dir; }
    double time() const { return tm; }

    glm::vec3 at(double t) const {
        return orig + static_cast<float>(t) * dir;
    }

  private:
    double tm;
    glm::vec3 orig;
    glm::vec3 dir;
};

#endif