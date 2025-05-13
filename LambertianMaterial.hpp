#include <glm/glm.hpp>
#include <random>
#include "Material.hpp"

class Lambertian : public Material {
  public:
    Lambertian(const glm::vec3& albedo) : albedo(albedo) {}

    bool scatter(const ray& r_in, const hit_record& rec, 
    ray& r_out, glm::vec3& attenuation, std::list<Light*>& lights, const glm::vec3& ambient)
    override {
        auto scatter_direction = rec.normal + random_unit_vector();
        if (near_zero(scatter_direction))
            scatter_direction = rec.normal;
        glm::vec3 newp = rec.front_face ? rec.p - 1e-8f * scatter_direction : rec.p + 1e-8f * scatter_direction;
        r_out = ray(newp, glm::normalize(scatter_direction));
        attenuation = albedo;
        // Texture stuff to be put in a conditional
		if (texture != nullptr) {
			glm::vec3 text_colour = texture->getColor(rec.u,rec.v);
			attenuation = text_colour;
		}
        return true;
    }

    glm::vec3 random_vec3(float min = -1.0f, float max = 1.0f) {
        static std::random_device rd; // Random device to seed the generator
        static std::mt19937 gen(rd()); // Mersenne Twister RNG
        std::uniform_real_distribution<float> dis(min, max); // Uniform distribution
    
        float x = dis(gen);
        float y = dis(gen);
        float z = dis(gen);
    
        return glm::vec3(x, y, z);
    }

    glm::vec3 random_unit_vector() {
        while (true) {
            auto p = random_vec3(-1,1);
            auto lensq = glm::dot(p,p);
            // random sampling until it fits our criteria
            if (1e-160 < lensq && lensq <= 1)
                return p / std::sqrt(lensq); // normalize it
        }
    }

    bool near_zero(glm::vec3& vec) {
        // Vector is close to zero vector
        float epsilon = 1e-7;
        return std::abs(vec.x) < epsilon && 
               std::abs(vec.y) < epsilon && 
               std::abs(vec.z) < epsilon;
    }

    ~Lambertian() {
        std::cout<<"deallocate"<<std::endl;
    }

  private:
    glm::vec3 albedo;
};