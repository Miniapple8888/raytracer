// #include "hittable.hpp"

// bool hittable::intersect_sphere(const ray&r, const float radius, const glm::vec3& center, double ray_tmin, double ray_tmax, hit_record& rec) {
//     glm::vec3 CQ = center - r.origin();
//     auto a = glm::dot(r.direction(), r.direction());
//     auto h = dot(r.direction(), CQ);
//     auto c = glm::dot(CQ,CQ) - radius*radius;

//     auto discriminant = h*h - a*c;
//     if (discriminant < 0)
//         return false;

//     auto sqrtd = std::sqrt(discriminant);

//     // Find the nearest root that lies in the acceptable range.
//     auto root = (h - sqrtd) / a;
//     if (root <= ray_tmin || ray_tmax <= root) {
//         root = (h + sqrtd) / a;
//         if (root <= ray_tmin || ray_tmax <= root)
//             return false;
//     }

//     rec.t = root;
//     rec.p = r.at(rec.t);
//     rec.normal = (rec.p - center) / static_cast<float>(radius);
//     rec.material = material;

//     glm::vec3 outward_normal = (rec.p - center) / static_cast<float>(radius);
//     rec.set_face_normal(r, outward_normal);

//     return true;
// }