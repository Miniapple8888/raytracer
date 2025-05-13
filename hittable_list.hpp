#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.hpp"
#include <iostream>
#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class hittable_list : public hittable {
  public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_tmax;

        int i =0;
        for (const auto& object : objects) {
            //if (object->material != nullptr) std::cout<<"NOT NULLPTR"<<std::endl;
            if (object->hit(r, ray_tmin, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t < closest_so_far ? temp_rec.t : closest_so_far;
                rec = temp_rec;
                rec.i = i;
            }
            i++;
        }

        return hit_anything;
    }

     void update(int delta_t) {
        // acceleration
        // doesn't do anything
        std::cout<<"update nothimng"<<std::endl;
    }

    void updateTf(const glm::mat4 &trans) {
        tf = trans;
    }
};

#endif
