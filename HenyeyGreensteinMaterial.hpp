#pragma once
#include <random>
#include "Material.hpp"

class HenyeyGreenstein : public Material {
public:
    // The Henyey-Greenstein phase function
    HenyeyGreenstein(const glm::vec3& albedo) : color(albedo) {}

    bool scatter(const ray& r_in, const hit_record& rec, 
    ray& r_out, glm::vec3& attenuation, std::list<Light*>& lights, const glm::vec3& ambient)
    override {
        // point through sphere, let it pass as if nothing were there
        auto dir_contd = r_in.direction();
        glm::vec3 pt = r_in.at(rec.t1);
        r_out = ray(pt, dir_contd);
        attenuation = color;
        return true;
    }

    float p(const float& cos_theta) {
        float denom = 1 + g * g - 2 * g * cos_theta; // check if denom is 0
        return 1 / (4 * (float)M_PI) * (1 - g * g) / (denom * sqrtf(denom));
    }

    float g = 0; // henyey-greenstein assymetry factor
    glm::vec3 color;
    float absorption = 0.1f; // absorption coefficient 
    float scattering = 0.1f; // scattering coefficient
    float extinction_coeff = absorption + scattering;
    float dx = 0.1f; // step size
    float density=0.25f;
    int d = 2; // random russian roulette 
    
    std::vector<float> transparencies;
    std::vector<glm::vec3> sample_poses;
};