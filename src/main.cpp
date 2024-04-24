#include "misc.h"
#include "hittable_list.h"
#include <iostream>
#include <image.h>
#include <sphere.h>
#include <triangle.h>

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
    std::vector<vec3> t = {vec3(0,0,-5), vec3(1,0,-5),vec3(1,1,-1)};
    std::vector<vec3> t2 = {vec3(-3,-3,-5), vec3(-1,0,-5),vec3(1,1,-1)};
    
    //Image attributes
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    //World attributes (objects in scene)

    hittable_list world;
    //world.add(make_shared<sphere>(point3(0,0,-1), 0.5));//adds a sphere with radius 0.5 at 0,0,-1
    world.add(make_shared<triangle>(t));
    world.add(make_shared<triangle>(t2));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    //Camera attributes
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    //Create viewport window based off origin
    point3 origin = point3(0,0,0);
    vec3 horizontal = vec3(viewport_width,0,0);
    vec3 vertical = vec3(0,viewport_height,0);
    vec3 lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0,0,focal_length);
    //make sure to not use auto, eigen does weird conversion and you lose things


    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height; j >= 0 ; j--){
        for(int i = 0; i<image_width; i++){
            
            auto u = double(i) / (image_width -1 );
            auto v = double(j) / (image_height -1 );
            //Create a ray originating from the camera origin, pointing towards the specified pixel on the viewport
            //The pixel is specified by the offset from the lower left corner of the viewport
            ray r(origin, (lower_left_corner + u*horizontal + v*vertical - origin));

            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);

        }
    }
}