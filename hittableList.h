//
// Created by ayush on 12/20/21.
//

#ifndef RAYTRACING_HITTABLELIST_H
#define RAYTRACING_HITTABLELIST_H

#include "raytracing.h"
#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittableList : public hittable {
public:
    hittableList() {}
    hittableList(shared_ptr<hittable> object) {add(object);}

    void clear() {objects.clear();}
    void add(shared_ptr<hittable> object) {objects.push_back(object);}

    virtual bool hit(const ray& r, double t_min, double t_max, hitRecord& rec) const override;

public:
    std::vector<shared_ptr<hittable>> objects;
};

bool hittableList::hit(const ray& r, double t_min, double t_max, hitRecord& rec) const {
    hitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif //RAYTRACING_HITTABLELIST_H
