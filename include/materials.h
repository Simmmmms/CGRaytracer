#ifndef MATERIALs_H
#define MATERIALs_H

#include "misc.h"
#include "texture.h"

struct hit_record;

// reflect function
double schlick(double cosine, double ref_idx)
{
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

// refract function
vec3 refract(const vec3 &uv, const vec3 &normal, double etai_over_etat)
{
    double cos_theta = fmin((-uv).dot(normal), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * normal);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.squaredNorm())) * normal;
    return r_out_perp + r_out_parallel;
}


class material
{
public:
    virtual color emitted(double u, double v, const point3& p) const {
        return color(0,0,0);
    }

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const = 0;
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

class lambertian : public material
{
public:
        lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
        lambertian(shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        vec3 scatter_direction = rec.normal + random_unit_vector();

        if (near_zero(scatter_direction))
        {
            scatter_direction = rec.normal;
        }

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

public:
    shared_ptr<texture> albedo;
};

class metal : public material
{
public:
    metal(const color &a) : albedo(a) {}

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        vec3 reflected = reflect(r_in.direction().normalized(), rec.normal);
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return (scattered.direction().dot(rec.normal) > 0);
    }

public:
    color albedo;
};

// glass material
class dielectric : public material
{
public:
    dielectric(float ri, color att) : refraction_index(ri), attenuation(att) {}

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        float etai_over_etat = (rec.front_face) ? (1.0 / refraction_index) : refraction_index;

        vec3 unit_direction = r_in.direction().normalized();
        float cos_theta = fmin((-unit_direction).dot(rec.normal), 1.0);
        float sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        if (etai_over_etat * sin_theta > 1.0)
        {
            vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = ray(rec.p, reflected);
            return true;
        }

        float reflect_prob = schlick(cos_theta, etai_over_etat);
        if (random_float() < reflect_prob)
        {
            vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = ray(rec.p, reflected);
            return true;
        }

        vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
        scattered = ray(rec.p, refracted);
        return true;
    }

public:
    float refraction_index;
    color attenuation;
};

// plastic material
class plastic : public material
{
public:
    plastic(const color &a, float f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        vec3 reflected = reflect(r_in.direction().normalized(), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (scattered.direction().dot(rec.normal) > 0);
    }

public:
    color albedo;
    float fuzz;
};

class diffuse_light : public material  {
    public:
        diffuse_light(shared_ptr<texture> a) : emit(a) {}
        diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            return false;
        }

        virtual color emitted(double u, double v, const point3& p) const override {
            return emit->value(u, v, p);
        }

    public:
        shared_ptr<texture> emit;
};

class checkered_compound : public material
{
public:
        checkered_compound(shared_ptr<material> a1, shared_ptr<material>a2, double s) : albedo1(a1), albedo2(a2), scale(s) {}

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        if(tilevalue(rec.u, rec.v, rec.p)){
            return albedo1->scatter(r_in,rec,attenuation,scattered);
        }else{
            return albedo2->scatter(r_in,rec,attenuation,scattered);
        }
    }

    virtual int tilevalue(double u, double v, const point3& p) const {
            auto sines = sin(scale*p.x())*sin(scale*p.y())*sin(scale*p.z());
            return sines<0;
        }

public:
    shared_ptr<material> albedo1;
    shared_ptr<material> albedo2;
    double scale;
};

#endif