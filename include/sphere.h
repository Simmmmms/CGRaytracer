#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include <eigen3/Eigen/Core>

class sphere: public hittable{//handles spheres in the scene
    public:
        sphere(){}
        sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r),mat_ptr(m) {};

        virtual bool hit(const ray&r, double t_min, double t_max, hit_record& rec) const override;

        public:
            point3 center;
            double radius;
            shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray&r, double t_min, double t_max, hit_record& rec) const{
    vec3 oc = r.origin() - center;
    auto a = r.direction().norm() * r.direction().norm();
    auto half_b = oc.dot(r.direction());
    auto c = (oc.norm() * oc.norm()) - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    auto root = (-half_b - sqrtd) / a;
    if(root < t_min || t_max < root){
        root = (-half_b + sqrtd) /a;
        if(root < t_min || t_max < root){
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;


}

#endif