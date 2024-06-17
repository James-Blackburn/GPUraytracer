#ifndef SCENES_HPP_INCLUDED
#define SCENES_HPP_INCLUDED

#include "hostStructs.hpp"
#include <vector>
#include <string>

int loadScene(std::vector<cl_Sphere>& spheres, const std::string& scenePath);

#endif