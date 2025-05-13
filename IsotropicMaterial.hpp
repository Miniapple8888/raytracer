// #include "Material.hpp"
// class Isotropic : public Material {
//   public:
//     Isotropic(const glm::vec3& albedo) : color(albedo) {}

//     bool scatter(const ray& r_in, const hit_record& rec, 
//     ray& r_out, glm::vec3& attenuation, std::list<Light*>& lights, const glm::vec3& ambient)
//     const override {
//         r_out = ray(rec.p, random_unit_vector(), r_in.time());
//         attenuation = color;
//         return true;
//     }

//     glm::vec3 random_vec3(float min = -1.0f, float max = 1.0f) {
//         static std::random_device rd; // Random device to seed the generator
//         static std::mt19937 gen(rd()); // Mersenne Twister RNG
//         std::uniform_real_distribution<float> dis(min, max); // Uniform distribution
    
//         float x = dis(gen);
//         float y = dis(gen);
//         float z = dis(gen);
    
//         return glm::vec3(x, y, z);
//     }

//     glm::vec3 random_unit_vector() {
//         while (true) {
//             auto p = random_vec3(-1,1);
//             auto lensq = glm::dot(p,p);
//             // random sampling until it fits our criteria
//             if (1e-160 < lensq && lensq <= 1)
//                 return p / std::sqrt(lensq); // normalize it
//         }
//     }

//   private:
//     glm::vec3 color;
// };