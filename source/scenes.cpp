#include "scenes.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

int loadScene(std::vector<cl_Sphere>& spheres, const std::string& scenePath) {
	std::ifstream file(scenePath);
	if (!file) {
		std::cerr << "Unable to open file." << std::endl;
		return -1;
	}
	
	std::string line;
	std::unordered_map<std::string, cl_Material> materials;
	while (std::getline(file, line)) {
		std::istringstream sstream(line);
		std::string identifier;
		sstream >> identifier;

		// Parse material
		if (identifier == "MATERIAL") {
			std::string materialName;
			sstream >> materialName;
			cl_Material& mat = materials[materialName];
			sstream >> mat.colour.x >> mat.colour.y >> mat.colour.z >> mat.fuzz >> mat.type;
		} 

		// Parse sphere
		else if (identifier == "SPHERE") {
			cl_Sphere sph;
			std::string materialName;
			sstream >> materialName;
			sph.material = materials[materialName];
			sstream >> sph.position.x >> sph.position.y >> sph.position.z >> sph.position.w;
			spheres.push_back(sph);
		}
	}

	return 0;
}