#ifndef MATERIALs_H
#define MATERIALs_H

#include "misc.h"

struct hit_record;

class material{
    public:
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        )const = 0;
};


// class default : public material{
//     public:
//         default(const color&a): albedo(a){}

//         virtual bool scatter(
//             const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
//         )const override{
//             vec3 scatter_direction = rec.normal + random_unit_vector();
//             scattered = ray(rec.p, scatter_direction);
//             attenuation = albedo;
//             return true;
//         }

//     public:
//         color albedo;
// };

class lambertian : public material{
    public:
        lambertian(const color&a): albedo(a){}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        )const override{
            vec3 scatter_direction = rec.normal + random_unit_vector();

            if(near_zero(scatter_direction)){
                scatter_direction = rec.normal;
            }

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    public:
        color albedo;
};

class metal : public material {
    public:
        metal(const color& a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(r_in.direction().normalized(), rec.normal);
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return (scattered.direction().dot(rec.normal) > 0);
        }

    public:
        color albedo;
};

#endif