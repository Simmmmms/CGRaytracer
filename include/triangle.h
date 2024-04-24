#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

class triangle: public hittable{
    public:
        triangle(){}
        triangle(std::vector<vec3>& vert) : verticies(vert){};

        virtual bool hit(const ray&r, double t_min, double t_max, hit_record& rec) const override;

        public:
            std::vector<vec3> verticies;

};

bool triangle::hit(const ray&r, double t_min, double t_max, hit_record& rec) const{

    vec3 A = verticies[1] - verticies[0]; //AB
    vec3 B = verticies[2] - verticies[0]; //AC
    vec3 tNorm = A.cross(B).normalized();//Triangles normal

        if(tNorm.dot(r.direction()) == 0){//Checks if the ray is perpendicular to the plane of the triangle
        return false;
    }


    auto D = -tNorm.dot(verticies[0]);//Distance between origin and plane
    auto t = -(tNorm.dot(r.origin())+D) / tNorm.dot(r.direction());

    if(t < t_min || t_max < t){
        return false;
    }

    if(t<0){//Checks if triangle is behind ray
        return false;
    }

    vec3 phit = r.origin() + t * r.direction();//point where ray intersects the plane


    vec3 c;

    vec3 edge0 = verticies[1] - verticies[0];
    vec3 c0 = phit - verticies[0];
    c = edge0.cross(c0);
    if(tNorm.dot(c) < 0){
        return false;
    }

    vec3 edge1 = verticies[2] - verticies[1];
    vec3 c1 = phit - verticies[1];
    c = edge1.cross(c1);
    if(tNorm.dot(c) < 0){
        return false;
    }

    vec3 edge2 = verticies[0] - verticies[2];
    vec3 c2 = phit - verticies[2];
    c = edge2.cross(c2);
    if(tNorm.dot(c) < 0){
        return false;
    }



    rec.t = t;
    rec.p = phit;
    rec.set_face_normal(r, tNorm);

    return true;


}

#endif