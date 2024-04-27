#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#include <limits>

class triangle: public hittable{
    public:
        triangle(){}
        triangle(std::vector<vec3>& vert, shared_ptr<material> m) : verticies(vert), mat_ptr(m){
            A = verticies[1] - verticies[2]; //AB
            B = verticies[2] - verticies[0]; //AC
            C = verticies[0] - verticies[1]; 
            tNorm = A.cross(B).normalized();//Triangles normal
            NA = tNorm.cross(A);
            NA = NA / NA.dot(C);

            NB = tNorm.cross(B);
            NB = NB / NB.dot(A);

            CA = NA.dot(verticies[1]);
            CB = NB.dot(verticies[2]);
            AdotN = tNorm.dot(verticies[0]);
            
        };

        virtual bool hit(const ray&r, double t_min, double t_max, hit_record& rec) const override;
        bool bounding_box(double time0, double time1, aabb& output_box) const override;

        public:
            std::vector<vec3> verticies;
            shared_ptr<material> mat_ptr;
            vec3 tNorm;
            vec3 A;
            vec3 B;
            vec3 C;
            vec3 NA;
            vec3 NB;
            float CA;
            float CB;
            float AdotN;

};

bool triangle::bounding_box(double time0, double time1, aabb& output_box) const {
    float minf = std::numeric_limits<float>::min();
    float maxf = std::numeric_limits<float>::max();
    vec3 min = vec3(maxf,maxf,maxf);
    vec3 max = vec3(minf,minf,minf);

    for(vec3 v: verticies){
        min[0] = fmaxf(min.x(), v.x());
        min[1] = fmaxf(min.y(), v.y());
        min[2] = fmaxf(min.z(), v.z());

        max[0] = fmaxf(max.x(), v.x());
        max[1] = fmaxf(max.y(), v.y());
        max[2] = fmaxf(max.z(), v.z());
    }

    output_box = aabb(min, max);
    return true;
}

bool triangle::hit(const ray&r, double t_min, double t_max, hit_record& rec) const{

    if(tNorm.dot(r.direction().normalized()) == 0){//Checks if the ray is perpendicular to the plane of the triangle
        return false;
    }

    float t = ((AdotN - tNorm.dot(r.origin())) / tNorm.dot(r.direction().normalized()));

    if(t < t_min || t_max < t){
        return false;
    }

    if(t<0){//Checks if triangle is behind ray
       return false;
    }

    vec3 phit = r.origin() + t * r.direction().normalized();//point where ray intersects the plane

    float a = NA.dot(phit) - CA;
    if(a<0||a>1){
        return false;
    }

    float b = NB.dot(phit) - CB;
    if(b<0||1-a-b < 0){
        return false;
    }




    rec.t = t;
    rec.p = phit;
    rec.set_face_normal(r, tNorm);
    rec.mat_ptr = mat_ptr;

    return true;


}

#endif