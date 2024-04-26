#include "misc.h"
#include "hittable_list.h"
#include <iostream>
#include <image.h>
#include <sphere.h>
#include <triangle.h>
#include <camera.h>
#include <materials.h>
#include <object_reader.h>
#include <omp.h>

color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    if (depth <= 0){
        return color(0,0,0);
    }

    if(world.hit(r,0.001,infinity,rec)){
        ray scattered;
        color attenuation;
        if(rec.mat_ptr->scatter(r,rec,attenuation,scattered)){
            return attenuation.asDiagonal() * ray_color(scattered, world, depth-1);
        }
        return color(0,0,0);
    }

    vec3 unit_direction = r.direction().normalized();
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}


int main(){
    auto material_trione = make_shared<metal>(color(0.8, 0.8, 0.8));


    std::vector<vec3> t = {vec3(-2,0,-1), vec3(2,0,-1),vec3(0,1,-5)};
    std::vector<vec3> t2 = {vec3(-2,0,-2), vec3(2,0,-2),vec3(0,1,-2)};

    auto material_sphere = make_shared<lambertian>(color(0.7,0.3,0.3));
    auto material_ground = make_shared<lambertian>(color(0.8,0.8,0.0));

    auto material_tritwo = make_shared<metal>(color(0.8, 0.6, 0.2));
    
    
    //Image attributes

    const auto aspect_ratio = (16.0 / 9.0);
    const int samples_per_pixel = 1;
    const int max_depth = 5;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    color image[image_height*image_width];

    //Camera
    camera camera;

    //World attributes (objects in scene)
    hittable_list world;

    //obj_reader objr = obj_reader("../cow.obj", material_tritwo, world);

    world.add(make_shared<sphere>(point3(0,.25,-2), .5, material_sphere));
    world.add(make_shared<triangle>(t, material_trione));
    world.add(make_shared<triangle>(t2, material_tritwo));
    world.add(make_shared<sphere>(point3(0,-.25,-2), .5, material_sphere));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 98, material_ground));




    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    #pragma omp parallel
    for (int j = image_height-1; j >= 0 ; j--){
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for(int i = 0; i<image_width; i++){
            color pixel_color(0,0,0);
            for(int s = 0; s < samples_per_pixel; ++s){
                //Create a ray originating from the camera origin, pointing towards the specified pixel on the viewport
                //The pixel is specified by the offset from the lower left corner of the viewport
                //anti aliasing is done by sampling pixel samples_per_pixel times
                //massively increases render time
                auto u = (i+random_double()) / (image_width -1 );
                auto v = (j+random_double()) / (image_height -1 );

                ray r = camera.get_ray(u,v);
                pixel_color += ray_color(r,world, max_depth);
            }
            image[(image_height-j-1) * image_width + i] = pixel_color;

        }
    }

    for(color c: image){
        write_color(std::cout, c,samples_per_pixel);
    }
    
}