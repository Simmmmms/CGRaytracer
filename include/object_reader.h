#ifndef OBJECT_READER_H
#define OBJECT_READER_H

#include <fstream>
#include "misc.h"
#include <iostream>
#include <sstream>
#include "triangle.h"


struct Face{
    int v1, v2, v3;
};

class obj_reader{
    public:
        obj_reader(std::string fname, shared_ptr<material> mat, hittable_list& w) : filename(fname), mat_ptr(mat), world(w){
            std::ifstream file(filename);
            if(file.is_open()){
                std::string line;
                while(std::getline(file, line)){
                    std::stringstream ss(line);
                    std::string identifier;
                    ss >> identifier;
                        if (identifier == "v") {
                            float x, y, z;
                            ss >> x >> y >> z;
                            vertices.push_back(vec3(x,y,z));
                        }
                        else if (identifier == "f") {
                            int v1, v2, v3;
                            ss >> v1 >> v2 >> v3;
                            faces.push_back({v1, v2, v3});
                            std::vector<vec3> t = {vertices.at(v1-1), vertices.at(v2-1), vertices.at(v3-1)};
                            world.add(make_shared<triangle>(t,mat_ptr));
                    }
                }file.close();
            }else{
                std::cerr << "Error opening file: "<< filename << std::flush;
                exit(0);
            }

        }

    public:
        std::string filename;
        std::vector<vec3> vertices;
        std::vector<Face> faces;
        shared_ptr<material> mat_ptr;
        hittable_list& world;

};




#endif