#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "SceneNode.hpp"
#include "GeometryNode.hpp"
#include "Primitive.hpp"
#include "PhongMaterial.hpp"
#include "HenyeyGreensteinMaterial.hpp"
#include "Light.hpp"
#include "Image.hpp"
#include "medium.h"

#include "ray.h"
#include "hittable.hpp"
#include "hittable_list.hpp"


class Camera {
public:
    Image m_image;
    Camera(SceneNode* root, Image& image, double time, const glm::vec3& eye,
	        const glm::vec3& view, const glm::vec3& up, 
            const double fovy, const glm::vec3& ambient, 
            const std::list<Light*> & lights);
    void render(double time);
private:
    glm::vec3 ray_colour(ray &r, hittable& h, int depth);
    ray get_ray(int x, int y);
    void write_colour(glm::vec3& colour, int x, int y);
    void process_model(SceneNode* root, const glm::mat4& tf);
    void process_scene();
    void update_scene(float time_step);
    glm::vec3 is_in_shadow(glm::vec3& pt, int cur_obj_i, std::list<Light*> &lights);
    glm::vec3 background_colour(ray &r);
    glm::vec3 render_volume(ray &r_in, hit_record &rec, HenyeyGreenstein *hen, std::shared_ptr<hittable> hmedium, float &transparency);

    double m_time; // duration of animation
    int num_frames_elapsed; // num frames elapsed
    int fps = 24; // frames per second

    hittable_list hlist;
    int max_depth = 10;
    float bias = 0.001f; // for issues like self-intersecting rays, etc.
    int n_samples_shadow = 5;

    SceneNode *root;
    size_t h;
    size_t w;
    // Camera parameter
    glm::vec3 eye;
	glm::vec3 view;
	glm::vec3 up;
    double fovy;
    int samples_per_pixel=5; // antialiasing (10)
    glm::vec3 pixel_delta_u = glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 pixel_delta_v = glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 viewport_tl = glm::vec3(0.0f,0.0f,0.0f); // viewport top left
    glm::vec3 pixel00_loc = glm::vec3(0.0f,0.0f,0.0f); // first pixel location
    // Lighting parameters  
    glm::vec3 ambient;
    std::list<Light*> lights;
};