#ifndef HOST_STRUCTS_HPP_INCLUDED
#define HOST_STRUCTS_HPP_INCLUDED

#include <CL/cl.h>

struct cl_Camera {
	cl_float3 pixel00Center = { 0.0f, 0.0f, 0.0f };
	cl_float3 pixelDeltaU = { 0.0f, 0.0f, 0.0f };
	cl_float3 pixelDeltaV = { 0.0f, 0.0f, 0.0f };
	cl_float3 cameraPosition = { 0.0f, 0.0f, 0.0f };
};

struct cl_Material {
	cl_float3 colour;
	cl_float fuzz;
	cl_uint type;
	cl_float padding[2];
};

struct cl_Sphere {
	cl_Material material;
	cl_float4 position;
};

enum MaterialTypes {
	MATERIAL_DIFFUSE = 0,
	MATERIAL_DIFFUSE_CHEQUERED = 1,
	MATERIAL_METALLIC = 2,
	MATERIAL_LIGHT_SOURCE = 3
};

#endif