#ifndef CAMERA_H
#define CAMERA_H

#include "misc.h"

class camera{
    public:
        camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspect_ratio){
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta/2);

            //Camera attributes
            //Create viewport window based off origin

            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;
            
            auto w = (lookfrom - lookat).normalized();
            auto u = vup.cross(w).normalized();
            auto v = w.cross(u);

            origin = lookfrom;
            horizontal = viewport_width * u;
            vertical = viewport_height * v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - w;
        }

        ray get_ray(double u, double v )const{
            return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
        }

    private:
        point3 origin;
        vec3 horizontal;
        vec3 vertical;
        vec3 lower_left_corner;
        

};

#endif