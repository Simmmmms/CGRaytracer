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
#include <bvh.h>

#ifdef WIN32_
    #include <windows.h>
#endif

color ray_color(const ray& r, const color& background, const hittable& world, int depth) {
    hit_record rec;

    if (depth <= 0){
        return color(0,0,0);
    }

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation.asDiagonal() * ray_color(scattered, background, world, depth-1);
}



int main(){
    auto material_trione = make_shared<metal>(color(0.8, 0.8, 0.8));
    auto material_dielectric = make_shared<dielectric>(1.5, color(.9,1,.9));
    auto checkered = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9), .6);
    color background(0.70, 0.80, 1.00);
    auto difflight = make_shared<diffuse_light>(color(20,20,20));

    auto solidcolor = make_shared<solid_color>(0.8,0.8,0.0);

    auto grey_metal = make_shared<metal>(color(.5,.5,.5));
    auto darkgreen_solid = make_shared<lambertian>(color(0.02,.3,0));
    auto cow_checkered_plane = make_shared<checkered_compound>(grey_metal,darkgreen_solid, .6);


    std::vector<vec3> t = {vec3(-2,0,-1), vec3(2,0,-1),vec3(0,1,-5)};
    std::vector<vec3> t2 = {vec3(-15,-6,-15), vec3(15,-6,-15),vec3(0,11,-15)};

    auto material_metal = make_shared<metal>(color(0.8, 0.8, 0.8));
    auto material_ground = make_shared<lambertian>(solidcolor);
    auto material_tritwo = make_shared<lambertian>(color(0.8, 0.6, 0.2));
    auto material_plastic = make_shared<plastic>(color(0.8, 0.6, 0.2), 1.0);
    
    
    //Image attributes

    const int samples_per_pixel = 1000;
    const int max_depth = 50;
    const int image_width = 400;
    const int image_height = 250;
    const int image_size = image_width * image_height;
    const auto aspect_ratio = image_width / image_height;
    shared_ptr<color[]> image(new color[image_size]);

    //Camera
    camera camera(point3(90,80,45), point3(35,15,-15), point3(0,1,0),90.0, aspect_ratio);
    // camera camera(point3(0,0,0), point3(0,0,-1), point3(0,1,0),90.0, aspect_ratio);

    //World attributes (objects in scene)

    hittable_list objects;
    obj_reader objr = obj_reader("../horse.obj", material_plastic, objects);
    obj_reader objrplane = obj_reader("../plane.obj", cow_checkered_plane, objects);


    objects.add(make_shared<sphere>(point3(60,100,5), 10, difflight));
    // objects.add(make_shared<triangle>(t, difflight));
    // objects.add(make_shared<triangle>(t2, material_trione));
    //  objects.add(make_shared<sphere>(point3(0,-.25,-2), .5, material_dielectric));
    // objects.add(make_shared<sphere>(point3(0,-100.5,-1), 98, material_ground));

    hittable_list world;
    world.add(make_shared<bvh_node>(objects, 0,1));




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
                
                if(samples_per_pixel == 1){
                    auto u = double(i) / (image_width -1 );
                    auto v = double(j) / (image_height -1 );
                    ray r = camera.get_ray(u,v);
                    pixel_color += ray_color(r, background, world, max_depth);
                }else{
                    auto u = (i+random_double()) / (image_width -1 );
                    auto v = (j+random_double()) / (image_height -1 );
                    ray r = camera.get_ray(u,v);
                    pixel_color += ray_color(r, background, world, max_depth);
                }
            }
            image[(image_height-j-1) * image_width + i] = pixel_color;

        }
    }

    for(int i =0; i<image_size; i++){
        write_color(std::cout, image[i],samples_per_pixel);
    }
    
}
