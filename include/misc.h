#ifndef MISC_H
#define MISC_H


#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "ray.h"
#include <eigen3/Eigen/Core>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//common things

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    return min + (max-min)*random_double();
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}


inline static vec3 randomv3() {
    return vec3(random_double(), random_double(), random_double());
}

inline static vec3 randomv3(double min, double max) {
    return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
}

vec3 random_in_unit_sphere() {
    while (true) {
        vec3 p = randomv3(-1,1);
        if ((p.norm() * p.norm()) >= 1) continue;
        return p;
    }
}

vec3 random_unit_vector(){
    return random_in_unit_sphere().normalized();
}

vec3 random_in_hemisphere(const vec3& normal) {
    vec3 in_unit_sphere = random_in_unit_sphere();
    if (in_unit_sphere.dot(normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

bool near_zero(vec3 v){
    // Return true if the vector is close to zero in all dimensions.
    const auto s = 1e-8;
    return (fabs(v.x()) < s) && (fabs(v.y()) < s) && (fabs(v.z()) < s);
}

vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*(v.dot(n)*n);
}


#endif