#include "scenes.hpp"
#include "helper.hpp"

void createScene(std::vector<cl_Sphere>& spheres, unsigned int scene) {
	switch (scene) {
	case SCN_CHEQUERED_ROOM_1:
		spheres.resize(10);
		spheres[0] = { chequeredWhite, {0.0f, -100.5f, -1.0f, 100.0f} }; // floor
		spheres[1] = { diffusePink, {0.0f, 0.0f, -1.0f, 0.5f} }; // middle sphere
		spheres[2] = { metalFuzzyGrey, {-1.0f, 0.0f, -1.0f, 0.5f} }; // left sphere
		spheres[3] = { metalShinyGrey, {1.0f, 0.0f, -1.0f, 0.5f} }; // right sphere
		spheres[4] = { diffuseWhite, {0.0f, 0.0f, -104.0f, 100.0f} }; // back wall
		spheres[5] = { diffuseRed, {-103.0f, 0.0f, -1.0f, 100.0f} }; // left wall
		spheres[6] = { diffuseBlue, {103.0f, 0.0f, -1.0f, 100.0f} }; // right wall
		spheres[7] = { diffuseBlack, {0.0f, 0.0f, 104.0f, 100.0f} }; // front wall
		spheres[8] = { diffuseWhite, {0.0f, 104.0f, -1.0f, 100.0f} }; // ceiling
		spheres[9] = { lightsource, {0.0f, 3.0f, -1.0f, 1.0f} }; // light source
		break;
	case SCN_MIRROR_ROOM_1:
		spheres.resize(8);
		spheres[0] = { chequeredWhite, {0.0f, -100.5f, -1.0f, 100.0f} }; // floor
		spheres[1] = { metalShinyGrey, {0.0f, 0.0f, -1.0f, 0.5f} }; // middle sphere
		spheres[2] = { metalShinyGrey, {0.0f, 0.0f, -104.0f, 100.0f} }; // back wall
		spheres[3] = { metalShinyGrey, {-103.0f, 0.0f, -1.0f, 100.0f} }; // left wall
		spheres[4] = { metalShinyGrey, {103.0f, 0.0f, -1.0f, 100.0f} }; // right wall
		spheres[5] = { metalShinyGrey, {0.0f, 0.0f, 104.0f, 100.0f} }; // front wall
		spheres[6] = { diffuseWhite, {0.0f, 104.0f, -1.0f, 100.0f} }; // ceiling
		spheres[7] = { lightsource, {0.0f, 3.0f, -1.0f, 1.0f} }; // light source
		break;
	default:
		printf("Invalid scene specified\n");
		displayProgramUse();
		exit(EXIT_FAILURE);
	}
}