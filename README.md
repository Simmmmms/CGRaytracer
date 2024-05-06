# C++ Raytracer

# Required librarys

[Eigen](https://gitlab.com/libeigen/eigen)
If using windows, place the eigen library files inside of the external folder, under the name eigen3.
On linux, install the library and it should be found by cmake.
Note: Running inside a virtual machine is ~1000x slower than running native on windows. 

# Usage

You much define the materials inside the main function, like so:
    auto material_dielectric = make_shared<dielectric>(1.5, color(.9,1,.9));
The type inside of the make_shared<type> refers to the material. The parameters inside the parenthesis are the attributes of the material.
There are 6 material types
1. lambertian - This is a basic diffuse material. It takes one parameter which is a color or texture.
2. metal - This is a reflective surface. It takes one parameter which is color.
3. dielectric - This is a see through or glass material. It takes two parameters, the refractive index and the color.
4. plastic - This is a fuzzed metalic texture. It takes two parmeters, the color and the amount of fuzziness.
5. diffuse_light - This is a material that emits light. It takes one parameter, the color. Colors with values of greater than (1,1,1) are neccesary for light to be emitted.
6. checkered_compund - This is a material that takes two other materials and turns them into a checkerboard pattern. It accepts 3 parameters, material1, materials2, and the scale.

# Attributes

There are 2 important image attributes
1. samples_per_pixel - This determines how many rays to cast for each pixel. High numbers create better results but take signifigantly longer.
2. max_depth - This determines the amount of bounces maximum for each ray. 

The camera has 5 attributes
1. lookfrom - The position of the camera
2. lookat - The location that the camera is looking.
3. vup - The up position from the camera
4. vfov - The field of view
5. aspect_ration - The aspect ratio of the image

# Other important parts

obj_readers are how you import .obj files. 

    obj_reader objr = obj_reader("../cow.obj", material_plastic, objects);
    
You must provide the filepath relative to the executable, the material of the object, and a reference to a hittable_list (objects in this case);
You can add multiple of these.

You can also directly add triangles or spheres

    objects.add(make_shared<sphere>(point3(60,100,5), 10, difflight));
    objects.add(make_shared<triangle>(t, difflight));
Where sphere takes 3 parameters: An origin, a radius, and a material

and triangle takes 2: A vector containing the 3 vertices of the triangles, and a material.

You should add the objects to a new hittable_list that contains a BVH like so

    world.add(make_shared<bvh_node>(objects, 0,1));

This is a data structure to aid performance. It is not neccesary to use but signifigantly increases performance.