#include <glm/glm.hpp>


class Particle {
public:
    glm::vec3 position, velocity, acceleration, force;
    float density;
    float pressure;
    uint16_t hash; 

    // void updatePos();
    // void updateForce();
};