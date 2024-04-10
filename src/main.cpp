#include <iostream>
#include <image.h>
#include <eigen3/Eigen/Core>

int main(){
    
    int image_width = 512;
    int image_height = 512;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int i = 0; i < image_height; i++){
        for(int j = 0; j<image_width; j++){
            
            auto pixel_color = color(double(i)/(image_width-1), double(j)/(image_height-1),0);
            write_color(std::cout, pixel_color);
        }
    }
}