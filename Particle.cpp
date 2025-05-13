#include <glm/glm.hpp>
#include "Particle.hpp"

// void Particle::updatePos() {
//     // compute acceleration and velocity
//     glm::vec3 acceleration = p->force / p->density + glm::vec3(0, g, 0);
//     p->velocity += acceleration * deltaTime;

//     // Update position
//     p->position += p->velocity * deltaTime;
// }

// void Particle::updateForce(float mass, float viscosity) {
//     //unit direction and length
//     float dist = sqrt(dist2);
//     glm::vec3 dir = glm::normalize(pj->position - pi->position);

//     //apply pressure force
//     glm::vec3 pressureForce = -dir * mass * (pi->pressure + pj->pressure) / (2 * pj->density) * spikyGrad;
//     pressureForce *= std::pow(h - dist, 2);
//     pi->force += pressureForce;

//     //apply viscosity force
//     glm::vec3 velocityDif = pj->velocity - pi->velocity;
//     glm::vec3 viscoForce = viscosity * mass * (velocityDif / pj->density) * spikyLap * (h - dist);
//     pi->force += viscoForce;
// }

