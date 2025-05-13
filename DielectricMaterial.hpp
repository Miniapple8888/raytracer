#include "glm/glm.hpp"
#include "Material.hpp"


class Dielectric : public Material {
public: 
    float transparency = 0.8f;
    Dielectric(glm::vec3& albedo, double idx_refraction) : m_albedo{albedo}, m_idx_refraction(idx_refraction) {}
    bool scatter(const ray& r_in, const hit_record& rec, ray& r_out, glm::vec3& attenuation, std::list<Light*>& lights, const glm::vec3& ambient)
    override {
        // refraction
        float epsilon=1e-8f;

        float ratio_ior = rec.front_face ? (1.0/m_idx_refraction) : m_idx_refraction;

        attenuation = m_albedo;
        // Texture stuff to be put in a conditional
		if (texture != nullptr) {
			glm::vec3 text_colour = texture->getColor(rec.u,rec.v);
			attenuation = text_colour;
		}
        
        kr = fresnel(r_in.direction(), rec.normal, m_idx_refraction);
        if (kr < 1.0f) { //if not total refraction
            glm::vec3 refracted_dir = refract(r_in.direction(), rec.normal, ratio_ior);
            glm::vec3 newp = rec.front_face ? rec.p - epsilon * refracted_dir : rec.p + epsilon * refracted_dir;
            ray refracted_ray(newp, glm::normalize(refracted_dir));
            r_out = refracted_ray;
        }
        glm::vec3 reflected = reflect(r_in.direction(), rec.normal, ratio_ior);
        glm::vec3 reflect_dir = glm::normalize(reflected);

        glm::vec3 newp = rec.front_face ? rec.p - epsilon * reflect_dir : rec.p + epsilon * reflect_dir;
        ray ref(newp, reflect_dir);
        reflected_ray = ref;

        return true;
    }
    float getKr() {
        return kr;
    }
    ray getReflected() {
        return reflected_ray;
    }
    glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n, float ratio_ior) {
        return v - 2*glm::dot(v,n)*n;
    }


    float fresnel(const glm::vec3& incident, const glm::vec3& normal, float ior) {
        float cosI = glm::dot(-incident, normal);
        float n1 = 1.0f; // from incident ray (air)
        float n2 = ior; // transmitted ray inside medium
        // shlick's approximation
        float R0 = glm::pow((n1-n2) / (n1+n2), 2);

        return R0 + (1.0f - R0) * glm::pow(1.0f - cosI, 5);
    }

    glm::vec3 refract(const glm::vec3& incident, const glm::vec3& normal, float ratio_ior) {
        float IN_dot = glm::dot(-incident, normal);
        float cosI = std::fmin(IN_dot, 1.0f);
        glm::vec3 R_perp = ratio_ior * (incident + IN_dot*normal);
        glm::vec3 R_parall = - glm::sqrt(std::fabs(1-glm::dot(R_perp,R_perp))) * normal;
        return R_perp + R_parall;
    }


private:
    glm::vec3 m_albedo;
    double m_idx_refraction;
    float kr = 0.0f; // no reflection
    ray reflected_ray = ray(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
};