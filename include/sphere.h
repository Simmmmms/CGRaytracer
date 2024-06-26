#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include <eigen3/Eigen/Core>

class sphere: public hittable{//handles spheres in the scene
    public:
        sphere(){}
        sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r),mat_ptr(m) {};

        virtual bool hit(const ray&r, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

        public:
            point3 center;
            double radius;
            shared_ptr<material> mat_ptr;

         private:
            static void get_sphere_uv(const point3& p, double& u, double& v) {

                auto theta = acos(-p.y());
                auto phi = atan2(-p.z(), p.x()) + pi;

                u = phi / (2*pi);
                v = theta / pi;
            }
};

bool sphere::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = aabb(
        center - vec3(radius, radius, radius),
        center + vec3(radius, radius, radius));
    return true;
}

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
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;

    return true;


}

#endif