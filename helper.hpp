#ifndef HELPER_HPP_INCLUDED
#define HELPER_HPP_INCLUDED

#define _CRT_SECURE_NO_WARNINGS 

#include <CL/cl.h>

cl_context initialiseContext(cl_device_id* device);
cl_kernel compileKernelFromFile(const char* filename, const char* kernelName, cl_context context, cl_device_id device);
void checkStatus(cl_int status, const char* message);
void writeToPPM(const char* filename, cl_float3* data, int width, int height, int maxVal);
void displayProgramUse();

#endif