#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <random>
#include <type_traits>
#include "Camera.hpp"
#include "Primitive.hpp"
#include "plane.h"
#include "Plane.hpp"
#include "Texture.hpp"
#include "PhongMaterial.hpp"
#include "LambertianMaterial.hpp"
#include "DielectricMaterial.hpp"
#include "HenyeyGreensteinMaterial.hpp"
#include "medium.h"
#include "sphere.h"
#include "cube.h"

float PI = glm::pi<float>();

double linear_to_gamma(double linear_component) {
    if (linear_component > 0)
        return std::sqrt(linear_component);
    return 0;
}

double calculateViewportHeight(const glm::vec3& eye, const glm::vec3& view, double fovy) {
    // assumes no tilt
	
	// Convert fovy from degrees to radians
    double fovyRadians = glm::radians(fovy);
	double distance = glm::abs(view.z);
    double viewport_height = 2.0 * distance * std::tan(fovyRadians / 2.0);
    
    return viewport_height;
}

double calculateViewportWidth(const double viewport_height, const double width, const double height) {
	return viewport_height * (double(width)/height);
}

void Camera::write_colour(glm::vec3& colour, int x, int y) {
	// TODO: clamp rgb values
	
    // Red: 
    m_image(x, y, 0) = linear_to_gamma((double)colour.x);
    // Green: 
    m_image(x, y, 1) = linear_to_gamma((double)colour.y);
    // Blue: 
    m_image(x, y, 2) = linear_to_gamma((double)colour.z);
}

glm::vec3 Camera::background_colour(ray &r) {
	// gradient blue (sky) with point blending
	// darker
    auto dir = glm::normalize(r.direction());
    auto a = 0.5*(dir.y + 1.0);
    return (1.0-a)*glm::vec3(0.01f, 0.01f, 0.3f) + a*glm::vec3(0.05f, 0.05f, 0.2f);
}


glm::vec3 mix(const glm::vec3& reflected, const glm::vec3& colour, const float r) {
	return r * reflected + (1-r) * colour;
}

glm::vec3 Camera::is_in_shadow(glm::vec3& pt, int cur_obj_i, std::list<Light*> &lights) {
	glm::vec3 shadow_color = glm::vec3(0.0f,0.0f,0.0f);
	for (auto& light : lights) {
		
		for (int j = 0; j < n_samples_shadow; j++) {
			glm::vec3 sample_light_pt = light->sample_pt_on_surface();
			glm::vec3 dir_to_light = glm::normalize(sample_light_pt - pt);
			ray shadow_ray(pt, dir_to_light);
			
			// casting shadow ray to see if it intersects with object
			// loop through objs in scene
			hit_record rec;
			float ray_tmin = bias;
			float ray_tmax = std::numeric_limits<float>::infinity();

			int i = 0;
			
			for (const auto& object : hlist.objects) {
				if (i != cur_obj_i) { // should not be itself
					if (object->hit(shadow_ray, ray_tmin, ray_tmax, rec)) {
						shadow_color += light->colour * light->intensity;
						break; 
					}
				}
				i++;
			}
		}
		shadow_color /= n_samples_shadow;
		if (shadow_color != glm::vec3(0.0f)) { // TODO: what about multiple light sources?
			return shadow_color;
		}
	}
	return shadow_color;
}

// glm::vec3 Camera::fog(ray &r) {
// 	std::shared_ptr<hittable> hobj = nullptr; // nothing for now
// 	hit_record rec;
// 	int i = 0;
// 	for (auto& h : hlist.objects) {
// 		if (h->hit(r, 0.001, std::numeric_limits<float>::infinity(), rec)) {
// 			// hit an object
// 			hobj = hlist.objects[i]; // retrieve obj through id
// 		}
// 		i++;
// 	}
// 	if (!hobj) {
// 		return background_colour(r);
// 	}
// 	float step_size = 0.1;
//     float absorption = 0.5;
//     float scattering = 0.5;
//     float density = 0.25;
//     float g = 0; // henyey-greenstein asymetry factor
//     uint8_t d = 2; // russian roulette "probability"

//     int ns = std::ceil((rec.t1 - rec.t) / step_size);
//     step_size = (rec.t1 - rec.t) / ns;

//     float transparency = 1; // initialize transmission to 1 (fully transparent)
//     glm::vec3 result = glm::vec3(0,0,0); // initialize volumetric sphere color to 0

//     // ray marching (0, ns)
//     for (int n = 0; n < ns; ++n) {

//         // Jiterring the sample position
//         float t = rec.t + step_size * (n + distribution(generator));
//         glm::vec3 sample_pos = ray_orig + t * ray_dir;

//         // compute sample transmission
//         float sample_attenuation = exp(-step_size * density * (scattering + absorption));
//         transparency *= sample_attenuation;

//         // In-scattering. Find distance light travels through volumetric sphere to the sample.
//         // Then use Beer's law to attenuate the light contribution due to in-scattering.
        
// 		if (hobj->hit(sample_pos, light_dir, isect_vol) && isect_vol.inside) {
//             float light_attenuation = exp(-density * isect_vol.t1 * (scattering + absorption));
//             float cos_theta = (ray_dir * light_dir);
//             result += light_color * light_attenuation * density * scattering * p(g, cos_theta) * transparency * step_size;
//         }

//         // Russian roulette
//         if (transparency < 1e-3) {
//             if (distribution(generator) > 1.f / d)
//                 break;
//             else
//                 transparency *= d;
//         }
//     }

//     // combine background color and volumetric sphere color
//     return background_color * transparency + result;
// }

// Perlin noise

int p[512] = {151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7, 225,
    140,  36, 103,  30,  69, 142,   8,  99,  37, 240,  21,  10,  23, 190,   6, 148,
    247, 120, 234,  75,   0,  26, 197,  62,  94, 252, 219, 203, 117,  35,  11,  32,
     57, 177,  33,  88, 237, 149,  56,  87, 174,  20, 125, 136, 171, 168,  68, 175,
     74, 165,  71, 134, 139,  48,  27, 166,  77, 146, 158, 231,  83, 111, 229, 122,
     60, 211, 133, 230, 220, 105,  92,  41,  55,  46, 245,  40, 244, 102, 143,  54,
     65,  25,  63, 161,   1, 216,  80,  73, 209,  76, 132, 187, 208,  89,  18, 169,
    200, 196, 135, 130, 116, 188, 159,  86, 164, 100, 109, 198, 173, 186,   3,  64,
     52, 217, 226, 250, 124, 123,   5, 202,  38, 147, 118, 126, 255,  82,  85, 212,
    207, 206,  59, 227,  47,  16,  58,  17, 182, 189,  28,  42, 223, 183, 170, 213,
    119, 248, 152,   2,  44, 154, 163,  70, 221, 153, 101, 155, 167,  43, 172,   9,
    129,  22,  39, 253,  19,  98, 108, 110,  79, 113, 224, 232, 178, 185, 112, 104,
    218, 246,  97, 228, 251,  34, 242, 193, 238, 210, 144,  12, 191, 179, 162, 241,
     81,  51, 145, 235, 249,  14, 239, 107,  49, 192, 214,  31, 181, 199, 106, 157,
    184,  84, 204, 176, 115, 121,  50,  45, 127,   4, 150, 254, 138, 236, 205,  93,
    222, 114,  67,  29,  24,  72, 243, 141, 128, 195,  78,  66, 215,  61, 156, 180,
	151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7, 225,
    140,  36, 103,  30,  69, 142,   8,  99,  37, 240,  21,  10,  23, 190,   6, 148,
    247, 120, 234,  75,   0,  26, 197,  62,  94, 252, 219, 203, 117,  35,  11,  32,
     57, 177,  33,  88, 237, 149,  56,  87, 174,  20, 125, 136, 171, 168,  68, 175,
     74, 165,  71, 134, 139,  48,  27, 166,  77, 146, 158, 231,  83, 111, 229, 122,
     60, 211, 133, 230, 220, 105,  92,  41,  55,  46, 245,  40, 244, 102, 143,  54,
     65,  25,  63, 161,   1, 216,  80,  73, 209,  76, 132, 187, 208,  89,  18, 169,
    200, 196, 135, 130, 116, 188, 159,  86, 164, 100, 109, 198, 173, 186,   3,  64,
     52, 217, 226, 250, 124, 123,   5, 202,  38, 147, 118, 126, 255,  82,  85, 212,
    207, 206,  59, 227,  47,  16,  58,  17, 182, 189,  28,  42, 223, 183, 170, 213,
    119, 248, 152,   2,  44, 154, 163,  70, 221, 153, 101, 155, 167,  43, 172,   9,
    129,  22,  39, 253,  19,  98, 108, 110,  79, 113, 224, 232, 178, 185, 112, 104,
    218, 246,  97, 228, 251,  34, 242, 193, 238, 210, 144,  12, 191, 179, 162, 241,
     81,  51, 145, 235, 249,  14, 239, 107,  49, 192, 214,  31, 181, 199, 106, 157,
    184,  84, 204, 176, 115, 121,  50,  45, 127,   4, 150, 254, 138, 236, 205,  93,
    222, 114,  67,  29,  24,  72, 243, 141, 128, 195,  78,  66, 215,  61, 156, 180};

float fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }
float lerp(float t, float a, float b) { return a + t * (b - a); }
float grad(int hash, float x, float y, float z)
{
    int h = hash & 15;
    float u = h < 8 ? x : y,
          v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float noise(float x, float y, float z)
{
    int X = static_cast<int>(std::floor(x)) & 255,
        Y = static_cast<int>(std::floor(y)) & 255,
        Z = static_cast<int>(std::floor(z)) & 255;
    x -= std::floor(x);
    y -= std::floor(y);
    z -= std::floor(z);
    float u = fade(x),
          v = fade(y),
          w = fade(z);
    int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z,
        B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

    return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
                                   grad(p[BA], x - 1, y, z)),
                           lerp(u, grad(p[AB], x, y - 1, z),
                                   grad(p[BB], x - 1, y - 1, z))),
                   lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
                                   grad(p[BA + 1], x - 1, y, z - 1)),
                           lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
                                   grad(p[BB + 1], x - 1, y - 1, z - 1))));
}

float smoothstep(float lo, float hi, float x) {
    float t = glm::clamp((x - lo) / (hi - lo), 0.f, 1.f);
    return t * t * (3.0 - (2.0 * t));
}

float eval_density(const glm::vec3& p, const glm::vec3& center, const float& radius, int num_frames_elapsed) { 
    glm::vec3 vp = p - center;
    glm::vec3 vp_xform;

    float theta = (num_frames_elapsed - 1) / 120.f * 2 * (float)M_PI;
    vp_xform.x =  cos(theta) * vp.x + sin(theta) * vp.z;
    vp_xform.y = vp.y;
    vp_xform.z = -sin(theta) * vp.x + cos(theta) * vp.z;

	float dist = std::min(1.f, vp.length() / radius);
	float falloff = smoothstep(0.8, 1, dist);
    float freq = 0.5;
	size_t octaves = 5;
	float lacunarity = 2;
	float H = 0.4;
    vp_xform *= freq;
	float fbmResult = 0;
	float offset = 0.75;
	for (size_t k = 0; k < octaves; k++) {
		fbmResult += noise(vp_xform.x , vp_xform.y, vp_xform.z) * pow(lacunarity, -H * k);
        vp_xform *= lacunarity;
	}
    return std::max(0.f, fbmResult) * (1 - falloff);//(1 - falloff);//std::max(0.f, fbmResult);// * (1 - falloff));
}


glm::vec3 Camera::render_volume(ray &r_in, hit_record &rec, HenyeyGreenstein *hen, std::shared_ptr<hittable> hmedium, float &transparency) {
	int ns = std::ceil((rec.t1 - rec.t) / hen->dx);
    float stride = (rec.t1 - rec.t) / ns; // distance traveled in medium

	auto hmed = std::dynamic_pointer_cast<medium>(hmedium);
	glm::vec3 center = hmed->sph->get_m_pos();
	float radius = hmed->sph->get_m_radius();
	glm::vec3 result = glm::vec3(0.0f,0.0f,0.0f);
	transparency = 1.0f;

	// ray marching (0, ns)
    for (int n = 0; n < ns; ++n) {

		// jitter sample pos
		float ra = ((double) rand() / (RAND_MAX)) + 1; // rand num from 0 to 1
		float t = rec.t + stride * (n + ra);
		glm::vec3 sample_pos = r_in.at(t);

		// compute sample transmission
		float density = eval_density(sample_pos, center, radius, num_frames_elapsed);
		float sample_attenuation = exp(-hen->dx * hen->density * hen->extinction_coeff);
		transparency *= sample_attenuation;
		
		if (density > 0) {
			for (auto& light : lights ) {
				glm::vec3 light_dir = glm::normalize(sample_pos - light->position); // light to sample point
				ray medtolight(light->position, light_dir);
				hit_record med_rec;
				// in scattering
				if (hmedium->hit(medtolight, 0.001, std::numeric_limits<float>::infinity(), med_rec)) {
					float cos_theta = glm::dot(r_in.direction(), light_dir);
					float phase = hen->p(cos_theta);

					size_t num_steps_light = std::ceil(rec.t1 / hen->dx);
            		float stide_light = rec.t1 / num_steps_light;
					float tau = 0.0f;

					for (size_t nl = 0; nl < num_steps_light; ++nl) {
						float t_light = stide_light * (nl + 0.5);
              			glm::vec3 light_sample_pos = sample_pos + light_dir * t_light;
						tau += eval_density(light_sample_pos, center, radius, num_frames_elapsed);
					}
					
					// result += light->colour * light_attenuation * hen->density * hen->scattering * phase * transparency * hen->dx;
					float light_ray_att = exp(-tau * stide_light * hen->extinction_coeff);
					result += light->colour * light_ray_att * phase *hen->scattering * transparency * stride * density;
				}
			}
		}
		
		// std::cout<<"every transp: "<<i<<" num:"<<transp_size<<" num increasing:"<<hen->transparencies.size() << std::endl;
		result *= transparency;

		// Russian roulette
		if (transparency < 1e-3) {
			ra = ((double) rand() / (RAND_MAX)) + 1;
			if (ra > 1.f / hen->d)
				break;
			else
				transparency *= hen->d;
		}
	}
	return result;
}

glm::vec3 Camera::ray_colour(ray &r, hittable& h, int depth) {
    glm::vec3 bg_colour = background_colour(r);
	if (depth < 0) { // stop recursing too far
		return bg_colour;
	}

	hit_record rec;
    if (h.hit(r, 0.001, std::numeric_limits<float>::infinity(), rec)) {
		ray r_out;
		glm::vec3 attenuation;
		if (rec.material->scatter(r, rec, r_out, attenuation, lights, ambient)) {

			if (typeid(*rec.material) == typeid(PhongMaterial)) {
				glm::vec3 colour = r_out.direction();
				// std::cout<<"Phong"<<std::endl;
				return colour;
			}
			if (typeid(*rec.material) == typeid(HenyeyGreenstein)) {
				auto hen = static_cast<HenyeyGreenstein*>(rec.material);
				auto hmedium = hlist.objects[rec.i];

				float transparency = 1.0f;
				glm::vec3 result = render_volume(r, rec, hen, hmedium, transparency);
				// std::cout<<"Fog"<<std::endl;
				// combine background color and volumetric sphere color
				return transparency * ray_colour(r_out, h, depth-1) + result;
			}
			if (typeid(*rec.material) == typeid(Lambertian)) {
				// loop through each light
				glm::vec3 vis = glm::vec3(1.0f) - is_in_shadow(rec.p, rec.i, lights);
				// glm::vec3 vis = glm::vec3(1.0f,1.0f,1.0f);
				glm::vec3 lambert_color = glm::vec3(0.0f,0.0f,0.0f);
				// glm::vec3 diffuse_color = ray_colour(r_out, h, depth-1);
				for (auto& light : lights) {
					glm::vec3 L = -light->dir;
					glm::vec3 light_dir;
					glm::vec3 light_intensity;
					float distance;
					light->illuminate(rec.p, light_dir, light_intensity, distance);
					lambert_color += vis * attenuation / M_PI * light->intensity * light->colour * std::max(0.f, glm::dot(rec.normal,-light_dir)); 
					// lambert_color += vis * attenuation * light_intensity * std::max(0.f, glm::dot(rec.normal,-light_dir));
				}
				// std::cout<<"Lambert colour"<<std::endl;
				return lambert_color;
			}
			if (typeid(*rec.material) == typeid(Dielectric)) {
				Dielectric* diel = static_cast<Dielectric*>(rec.material);
				float kr = diel->getKr();
				// calculate reflected
				ray reflected = diel->getReflected();
				glm::vec3 reflectedColor = ray_colour(reflected, h, depth-1);
				glm::vec3 refractionColor = glm::vec3(0.0f,0.0f,0.0f);
				if (kr < 1.0f) { // if not total reflection, we can refract
					refractionColor = ray_colour(r_out, h, depth-1);
				}
				glm::vec3 fresnel_colour = reflectedColor * kr + refractionColor * (1 - kr);
				glm::vec3 total_colour = attenuation * diel->transparency * fresnel_colour;
				// std::cout<<"dielectric"<<std::endl;
				return total_colour * attenuation;
			}
			glm::vec3 metal = attenuation * ray_colour(r_out, h, depth-1);
			// std::cout<<"metal"<<std::endl;
			return metal;
		}
    }
	return bg_colour;
}

double random_double() {
    // Returns a random real in [0,1).
    static std::random_device rd;  // Seed for random number generator
    static std::mt19937 gen(rd()); // Mersenne Twister random number generator
    static std::uniform_real_distribution<> dis(0.0, 1.0); // Range [0, 1)
	return dis(gen);
}
ray Camera::get_ray(int x, int y) {
	auto ray_time = random_double();
	double offset_x = random_double() - 0.5;
	double offset_y = random_double() - 0.5;
    auto pixel_sample = pixel00_loc
                      + ((x + offset_x) * pixel_delta_u)
                      + ((y + offset_y) * pixel_delta_v);
    auto pixel_center = pixel_sample;
    auto ray_direction = glm::normalize(pixel_center - eye);
    // if ( y == h/2 && x == w/2) {
    //     std::cout<< "y:"<<std::to_string(y)<<" x:"<<std::to_string(x)<<std::endl;
    //     std::cout << "Ray direction: " << glm::to_string(ray_direction) << std::endl;
    // }
    return ray(eye, ray_direction, ray_time); // create a ray
}

Camera::Camera(SceneNode* root, Image& image, double time, const glm::vec3& eye,
	        const glm::vec3& view, const glm::vec3& up, 
            const double fovy, const glm::vec3& ambient, 
            const std::list<Light *> & lights) : root{root}, m_image{image}, m_time(time),
            eye{eye}, view{view}, up{up}, fovy{fovy}, ambient{ambient}, lights{lights}  {
    h = m_image.height();
	w = m_image.width();

	double viewport_height = calculateViewportHeight(eye, view, fovy);
	double viewport_width = calculateViewportWidth(viewport_height,w,h);

	glm::vec3 viewport_u = glm::vec3(viewport_width, 0.0f, 0.0f);
	glm::vec3 viewport_v = glm::vec3(0.0f, -viewport_height, 0.0f);

	std::cout<<"Viewport u: " << glm::to_string(viewport_u) << std::endl;
	std::cout<<"Viewport v: " << glm::to_string(viewport_v) << std::endl;

	pixel_delta_u = viewport_u / w;
    pixel_delta_v = viewport_v / h;
	
	viewport_tl = eye + view - viewport_u/2 - viewport_v/2;
	pixel00_loc = viewport_tl + 0.5 * (pixel_delta_u + pixel_delta_v);
	std::cout<<"viewport_tl: " << glm::to_string(viewport_tl) << std::endl;
	std::cout<<"Pixel00_loc: " << glm::to_string(pixel00_loc) << std::endl;

    process_scene();
}

void Camera::process_model(SceneNode* node, const glm::mat4& tf) {
	for (SceneNode* child : node->children) {
		if (child->m_nodeType == NodeType::GeometryNode) {
			GeometryNode* geom_node = static_cast<GeometryNode*>(child);
			Primitive* prim = geom_node->m_primitive;
			geom_node->m_material->texture = geom_node->getTexture();
			if (typeid(*prim) == typeid(NonhierSphere)) {
				NonhierSphere* sph = static_cast<NonhierSphere*>(prim);
				sph->rb = geom_node->getRigidbody();
				hlist.add(std::make_shared<sphere>(sph, geom_node->m_material, tf * geom_node->trans));
			}
			if (typeid(*prim) == typeid(MediumSphere)) {
				MediumSphere* sph = static_cast<MediumSphere*>(prim);
				hlist.add(std::make_shared<medium>(sph, geom_node->m_material, tf * geom_node->trans));
			}
			if (typeid(*prim) == typeid(Sphere)) {
				Sphere* sph = static_cast<Sphere*>(prim);
				hlist.add(std::make_shared<sphere>(sph, geom_node->m_material, tf * geom_node->trans));
			}
			if (typeid(*prim) == typeid(Cube)) {
				Cube* b = static_cast<Cube*>(prim);
				hlist.add(std::make_shared<cube>(b, geom_node->m_material, tf * geom_node->trans));
			}
			if (typeid(*prim) == typeid(NonhierBox)) {
				NonhierBox* b = static_cast<NonhierBox*>(prim);
				hlist.add(std::make_shared<cube>(b, geom_node->m_material, tf * geom_node->trans));
			}
			if (typeid(*prim) == typeid(Mesh)) {
				Mesh* m = static_cast<Mesh*>(prim);
				hlist.add(std::make_shared<mesh>(m, geom_node->m_material, tf * geom_node->trans));
			}
			if (typeid(*prim) == typeid(Plane)) {
				Plane* m = static_cast<Plane*>(prim);
				hlist.add(std::make_shared<plane>(m, geom_node->m_material, tf * geom_node->trans));
			}
		} else if (child->m_nodeType == NodeType::SceneNode) {
			process_model(child, tf);
		} else if (child->m_nodeType == NodeType::JointNode) {
			process_model(child, tf);
		}
	}
}

void Camera::process_scene() {
	std::cout<<"Start processing scene"<<std::endl;
	process_model(root, root->trans);
	std::cout<<"Finished processing scene"<<std::endl;
}

bool spheresIntersect(glm::vec3 center1, float radius1, glm::vec3 center2, float radius2) {
    // Calculate the distance between the centers of the spheres
    float distance = glm::length(center2 - center1);

	//TODO: add epsilon to account for float point error
	float epsilon = 1e-8f;
    
    // Check if the distance is less than or equal to the sum of the radii
    return distance <= (radius1 + radius2 + epsilon);
}

void resolveCollision(RigidBody* rA, RigidBody* rB, glm::vec3& normal, double delta_t) {
	glm::vec3 relative_v = rB->v - rA->v; // difference of velocity
	float relv_along_normal = glm::dot(relative_v, normal);
	if (relv_along_normal > 0) { //separating
		return;
	}
	float e = glm::min(rA->e, rB->e); // coefficient restitution (bounciness)
	// calculate impulse scalar
	float j = -(1+e)*relv_along_normal;
	j /= 1/rA->m + 1/rB->m;
	// apply impulse
	glm::vec3 impulse = j * normal;
	rA->v -= impulse / rA->m;
	auto inc = rB->v + impulse / rB->m;
	rB->v = inc;
	auto y = rB->v;
}

void Camera::update_scene(float time_step) {
	glm::vec3 center = glm::vec3(0.0f,0.0f,0.0f);
	float radius = -1.0f;
	std::shared_ptr<sphere> prev_s;
	int i = 0;
	for (auto& h : hlist.objects) {
		glm::mat4 tf(1.0f);
		// tf = glm::translate(tf, glm::vec3(1.0f, -1.0f, 1.0f));
		// h->updateTf(tf);
		// collision detection
		if (typeid(*h) == typeid(sphere)) {
			auto s = std::dynamic_pointer_cast<sphere>(h);
			glm::vec3 curcenter = s->sph->get_m_pos();
			float curradius = s->sph->get_m_radius();
			
			if (radius >= 0.0f && (s->sph->rb->type == "dynamic" || prev_s->sph->rb->type == "dynamic")) { // we have a prev sphere stored
				if (spheresIntersect(center, radius, curcenter, curradius)) {
					glm::vec3 n = glm::normalize(curcenter-center); // normal
					resolveCollision(prev_s->sph->rb, s->sph->rb, n, time_step);
					// glm::vec3 dir = glm::normalize(curcenter - center);
					// sphere2_position = sphere1_position + direction * (sphere1_radius + sphere2_radius + 0.1f)
					std::cout<<"sphere intersect"<<std::endl;
				}
			}

			curcenter = s->sph->rb->updateA(curcenter,time_step);
			
			s->motionBlurUpdate(curcenter); // TODO: check if change of pos


			center = curcenter;
			radius = curradius;
			prev_s = s;
		}
		i++;
	}
}



void Camera::render(double time) {
	// for each frame
	int num_frames = (int)time * fps;
	for (int t = 0; t < num_frames; t++) {
		// update scene geometry at each frame
		update_scene(0.5); // instead of delta t

		// loop through each pixel in the screen to render
		for (uint y = 0; y < h; ++y) {
			for (uint x = 0; x < w; ++x) {
				glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f);
				// if ( y == 0 && x == 0) {
				for (int i = 0; i < samples_per_pixel; i++) {
					ray pixel_ray = get_ray(x,y);
					glm::vec3 interm_col = ray_colour(pixel_ray, hlist, max_depth);
					color += interm_col;
				}
				color /= samples_per_pixel;
				// }
				// std::cout<<"Progress ("<<std::to_string(x)<<","<<std::to_string(y)<<")"<<std::endl;
				//std::cout<<"Progress: "<<std::to_string(((y+1)*w+(x+1))/(w*h)*100)<<"%"<<std::endl;
				write_colour(color, x, y);
			}
		}
		std::string filename = "out/out" + std::to_string(t) + ".png";
		m_image.savePng(filename);
		num_frames_elapsed++;
		// save each frame
		std::cout<<"saved frame "<<filename<<std::endl;
	}
	
}

