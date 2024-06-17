#ifndef SCENES_HPP_INCLUDED
#define SCENES_HPP_INCLUDED

#include "hostStructs.hpp"
#include <vector>

enum MaterialTypes {
	MAT_DIFFUSE,
	MAT_DIFFUSE_CHEQUERED,
	MAT_METALLIC,
	MAT_LIGHT_SOURCE
};

enum Scenes {
	SCN_CHEQUERED_ROOM_1,
	SCN_MIRROR_ROOM_1,
};

// MATERIALS
constexpr cl_Material diffuseGrey = { {0.8f, 0.8f, 0.8f}, 0.0f, MAT_DIFFUSE };
constexpr cl_Material chequeredWhite = { {0.9f, 0.9f, 0.9f}, 0.0f, MAT_DIFFUSE_CHEQUERED };
constexpr cl_Material diffusePink = { {0.9f, 0.0f, 0.3f}, 0.0f, MAT_DIFFUSE };
constexpr cl_Material diffuseGreen = { {0.0f, 0.9f, 0.0f}, 0.0f, MAT_DIFFUSE };
constexpr cl_Material metalShinyGrey = { {0.8f, 0.8f, 0.8f}, 0.0f, MAT_METALLIC };
constexpr cl_Material metalFuzzyGrey = { {0.8f, 0.8f, 0.8f}, 0.25f, MAT_METALLIC };
constexpr cl_Material diffuseBlack = { {0.025f, 0.025f, 0.025f}, 0.0f, MAT_DIFFUSE };
constexpr cl_Material diffuseRed = { {0.9f, 0.0f, 0.0f}, 0.0f, MAT_DIFFUSE };
constexpr cl_Material diffuseBlue = { {0.0f, 0.0f, 0.9f}, 0.0f, MAT_DIFFUSE };
constexpr cl_Material diffuseWhite = { {0.9f, 0.9f, 0.9f}, 0.0f, MAT_DIFFUSE };
constexpr cl_Material lightsource = { {2.0f, 2.0f, 2.0f}, 0.0f, MAT_LIGHT_SOURCE };

void createScene(std::vector<cl_Sphere>& spheres, unsigned int scene);

#endif