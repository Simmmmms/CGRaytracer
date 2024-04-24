#include "misc.h"
#include "hittable_list.h"
#include <iostream>
#include <image.h>
#include <sphere.h>
#include <triangle.h>
#include <camera.h>

color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if(world.hit(r,0,infinity,rec)){
                return 0.5 * (rec.normal + color(1,1,1));
    }
    vec3 unit_direction = r.direction().normalized();
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}


int main(){
    std::vector<vec3> t = {vec3(-2,0,-1), vec3(2,0,-1),vec3(0,1,-5)};
    std::vector<vec3> t2 = {vec3(-2,0,-2), vec3(2,0,-2),vec3(0,1,-2)};
    
    //Image attributes

    const auto aspect_ratio = (16.0 / 9.0);
    const int samples_per_pixel = 40;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    //Camera
    camera camera;

    //World attributes (objects in scene)
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,.25,-2), .5));
    world.add(make_shared<triangle>(t));
    world.add(make_shared<triangle>(t2));
    world.add(make_shared<sphere>(point3(0,-.25,-2), .5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 98));




    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height; j >= 0 ; j--){
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
                pixel_color += ray_color(r,world);
            }
            
            write_color(std::cout, pixel_color,samples_per_pixel);

        }
    }
}