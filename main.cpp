#include "raytracing.h"

#include "color.h"
#include "hittableList.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>
#include <fstream>

color ray_color(const ray& r, const hittable& world, int depth) {
    if (depth <= 0) {
        return color(0, 0,0);
    }

    hitRecord rec;
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return color(0,0,0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t)*color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

hittableList random_scene() {
    hittableList world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

int main() {
    // Image
    const auto aspectRatio = 3.0 / 2.0;
    const int imageWidth = 1200;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 500;
    const int maxDepth = 50;

    // World

    auto world = random_scene();

    // Camera
    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspectRatio, aperture, dist_to_focus);

    // Render
    std::ofstream outputFile ("output.ppm");
    outputFile << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = imageHeight - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
        for (int i = 0; i < imageWidth; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samplesPerPixel; ++s) {
                auto u = (i + random_double()) / (imageWidth - 1);
                auto v = (j + random_double()) / (imageHeight - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, maxDepth);
            }
            write_color(outputFile, pixel_color, samplesPerPixel);
        }
    }

    outputFile.close();

    std::cerr << "\nDone.\n";
}
