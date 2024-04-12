#include <iostream>
#include <image.h>
#include <eigen3/Eigen/Core>
#include <ray.h>

bool hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = r.direction().dot(r.direction());
    auto b = 2.0 * oc.dot(r.direction());
    auto c = oc.dot(oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}

color ray_color(const ray& r) {
    if (hit_sphere(point3(0,0,-1), 0.5, r))
        return color(1, 0, 0);

    vec3 unit_direction = r.direction().normalized();
    auto t = 0.5*(-unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}


int main(){
    
    //Image attributes
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

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

    for (int j = 0; j < image_height; j++){
        for(int i = 0; i<image_width; i++){
            
            auto u = double(i) / (image_width -1 );
            auto v = double(j) / (image_height -1 );
            //Create a ray originating from the camera origin, pointing towards the specified pixel on the viewport
            //The pixel is specified by the offset from the lower left corner of the viewport
            ray r(origin, (lower_left_corner + u*horizontal + v*vertical - origin).normalized());

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);

        }
    }
}