#ifndef IMAGE_H
#define IMAGE_h

#include <eigen3/Eigen/Core>

#include <iostream>

using color = Eigen::Vector3f;

void write_color(std::ostream& out, const color& pixel_color){
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    std::cout << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif