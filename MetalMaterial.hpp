#include <glm/glm.hpp>
#include "Material.hpp"

glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n) {
    return v - 2*dot(v,n)*n;
}

class Metal : public Material {
public:
    double m_reflective_factor;
    Metal(glm::vec3 &albedo, double reflective_factor) : 
        m_albedo(albedo),
        m_reflective_factor(reflective_factor) {
    }
    bool scatter(const ray& r_in, const hit_record& rec, ray& r_out, glm::vec3& attenuation, std::list<Light*>& lights, const glm::vec3& ambient)
    override {
        // reflect
        float epsilon = 1e-8f;
        glm::vec3 reflection_dir = reflect(r_in.direction(), rec.normal);
        ray reflected_ray = ray(rec.p, glm::normalize(reflection_dir));
        attenuation = m_albedo;
        // Texture stuff to be put in a conditional
		if (texture != nullptr) {
			glm::vec3 text_colour = texture->getColor(rec.u,rec.v);
			attenuation = text_colour;
		}
        r_out = reflected_ray;
        return true;
    }
private:
    glm::vec3 m_albedo;
};