#pragma once
#include <string>
#include <glm/glm.hpp>

class RigidBody {
public:
    RigidBody(std::string dyn, float mass) : type(dyn), m(mass) {
        if (type != "dynamic") {
            v = glm::vec3(0,0,0);
        }
    }

    glm::vec3 updateA(glm::vec3& pos, float delta_t) {
        if (type == "dynamic") {
            // gravitational acceleration
            // glm::vec3 a = glm::vec3(0.0f,0.0f,0.0f);
            v += a * (float)delta_t;
            pos += v * (float)delta_t;
            m_pos = pos;
            return pos;
        } else if (type == "kinematic") {
            v = glm::vec3(0,0,0);
        } else if (type == "circle") {
            return updateCircle(pos, delta_t);
        } else if (type == "translation") {
            return updateTranslation(pos, delta_t);
        }
        m_pos = pos;
        return pos;
    }

    glm::vec3 updateCircle(glm::vec3& pos, float delta_t) {
        float radiusCircle = 100.0f; // circle radius of path we want to follow
        glm::vec3 centerCircle = glm::vec3(0.0f,0.0f,0.0f);
        t += delta_t * 0.75;
        pos.x = centerCircle.x + radiusCircle * glm::cos(t);
        pos.y = centerCircle.y + radiusCircle * glm::sin(t);
        m_pos = pos;
        return pos;
    }

    glm::vec3 updateTranslation(glm::vec3& pos, float delta_t) {
        float speed = 2.0f; //independent from velocity
        pos.x = 500.0f * std::sin(speed * t);
        t += delta_t * 0.01;
        m_pos = pos;
        return pos;
    }

    glm::vec3 update(glm::vec3& pos, float delta_t) {
        // no acceleration
        pos += v * (float)delta_t;
        return pos;
    }

    float e = 0.8f; // coefficient of restitution

    float g = 9.8f; // gravitational force
    float m = 12.0f; // what do we do with this
    std::string type = "static"; 
    // we can probably let the user set the velocity
    float t = 0.0f; // time cumulative
    glm::vec3 m_pos = glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 v = glm::vec3(0.0f,-12.0f,0.0f); // velocity
    glm::vec3 a = glm::vec3(0.0f, 0, 0.0f); // acceleration
};