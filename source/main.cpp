#define CL_TARGET_OPENCL_VERSION 120
#define _CRT_SECURE_NO_WARNINGS 

#include "helper.hpp"
#include "camera.hpp"
#include "kernelProgram.hpp"
#include "scenes.hpp"

#include <cstdio>
#include <vector>
#include <fstream>

constexpr unsigned int LOCAL_WG_SIZE_X = 8;
constexpr unsigned int LOCAL_WG_SIZE_Y = 8;

inline float min(float a, float b) {
	return (a < b) ? a : b;
}

void writeToPPM(const char* filename, cl_float3* data, int width, int height, int maxVal) {
	std::ofstream file(filename, std::ios::binary);

	// write header info
	file << "P6\n" << width << " " << height << "\n" << maxVal << "\n";

	std::vector<unsigned char> image(width * height * 3);
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			image[(row * width + col) * 3] = floor(255.999f * min(data[row * width + col].x, 1.0f));
			image[(row * width + col) * 3 + 1] = floor(255.999f * min(data[row * width + col].y, 1.0f));
			image[(row * width + col) * 3 + 2] = floor(255.999f * min(data[row * width + col].z, 1.0f));
		}
	}
	
	// write image data
	file.write(reinterpret_cast<char*>(image.data()), image.size());
	file.close();
}

void raytraceScene(KernelProgram& kpRaytrace, std::vector<cl_Sphere>& spheres, int renderWidth, int renderHeight,
			  cl_uint samples, cl_uint batchSize, cl_uint maxDepth) {
	cl_int status;
	cl_event kernelEvent;
	cl_ulong startTime, endTime;
	double elapsedTime = 0.0;

	// calculate workgroup sizes
	const size_t GLOBAL_WG_SIZE[2] = {static_cast<size_t>(renderWidth), static_cast<size_t>(renderHeight)};
	const size_t LOCAL_WG_SIZE[2] = {LOCAL_WG_SIZE_X, LOCAL_WG_SIZE_Y};

	// initialise camera
	Camera camera({ 0.0f, 0.25f, 0.5f }, 90.0f, 0.0f);
	cl_Camera cameraProjection = camera.getProjection(renderWidth, renderHeight);
	cl_mem cameraBuffer = clCreateBuffer(kpRaytrace.getContext(),
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_Camera), &cameraProjection, &status);
	checkStatus(status, "Failed to create camera buffer");

	// create scene
	const cl_uint N_SPHERES = spheres.size();
	cl_mem spheresBuffer = clCreateBuffer(kpRaytrace.getContext(), 
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_Sphere) * N_SPHERES, spheres.data(), &status);
	checkStatus(status, "Failed to create sphere buffer");

	// initialise empty canvas for rendering
	std::vector<cl_float3> render(renderWidth * renderHeight, { 0.0f, 0.0f, 0.0f });
	cl_mem renderBuffer = clCreateBuffer(kpRaytrace.getContext(), 
		CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float3) * render.size(), render.data(), &status);
	checkStatus(status, "Failed to create render buffer");

	// set kernel args
	clSetKernelArg(kpRaytrace.getKernel("traceSamples"), 0, sizeof(cl_mem), &cameraBuffer);
	clSetKernelArg(kpRaytrace.getKernel("traceSamples"), 1, sizeof(cl_mem), &spheresBuffer);
	clSetKernelArg(kpRaytrace.getKernel("traceSamples"), 2, sizeof(cl_mem), &renderBuffer);
	clSetKernelArg(kpRaytrace.getKernel("traceSamples"), 3, sizeof(cl_uint), &N_SPHERES);
	clSetKernelArg(kpRaytrace.getKernel("traceSamples"), 4, sizeof(cl_uint), &maxDepth);
	clSetKernelArg(kpRaytrace.getKernel("averageSamples"), 0, sizeof(cl_mem), &renderBuffer);
	clSetKernelArg(kpRaytrace.getKernel("averageSamples"), 1, sizeof(cl_uint), &samples);

	// render the image
	for (cl_uint samplesCompleted = 0; samplesCompleted < samples; samplesCompleted += batchSize) {
		cl_uint sampleCount = (samplesCompleted + batchSize <= samples) ? batchSize : (samples - samplesCompleted);
		cl_uint seed = rand();

		clSetKernelArg(kpRaytrace.getKernel("traceSamples"), 5, sizeof(cl_uint), &sampleCount);
		clSetKernelArg(kpRaytrace.getKernel("traceSamples"), 6, sizeof(cl_uint), &seed);

		// enqueue kernel
		status = clEnqueueNDRangeKernel(kpRaytrace.getCommandQueue(), kpRaytrace.getKernel("traceSamples"), 
			2, NULL, GLOBAL_WG_SIZE, LOCAL_WG_SIZE, 0, NULL, &kernelEvent);
		checkStatus(status, "Failed to enqueue kernel");

		// Calculate time taken to execute kernel
		clWaitForEvents(1, &kernelEvent);
		clGetEventProfilingInfo(kernelEvent, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &startTime, NULL);
		clGetEventProfilingInfo(kernelEvent, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &endTime, NULL);
		elapsedTime += endTime - startTime;
		printf("Kernel:(%d) took: %.3f ms to execute\n", samplesCompleted/ batchSize, (double)(endTime - startTime) / 1.0e6);
	}

	// combine the samples
	status = clEnqueueNDRangeKernel(kpRaytrace.getCommandQueue(), kpRaytrace.getKernel("averageSamples"),
		2, NULL, GLOBAL_WG_SIZE, LOCAL_WG_SIZE, 0, NULL, &kernelEvent);
	checkStatus(status, "Failed to enqueue kernel");

	// Calculate time taken to execute kernel
	clWaitForEvents(1, &kernelEvent);
	clGetEventProfilingInfo(kernelEvent, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &startTime, NULL);
	clGetEventProfilingInfo(kernelEvent, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &endTime, NULL);
	elapsedTime += endTime - startTime;
	printf("Kernel took: %0.3f ms in total to execute\n", elapsedTime / 1.0e6);

	// enqueue read
	status = clEnqueueReadBuffer(kpRaytrace.getCommandQueue(), renderBuffer, CL_TRUE, 0,
		sizeof(cl_float3) * render.size(), render.data(), 0, NULL, NULL);
	checkStatus(status, "Failed to enqueue read operation");

	// free mem
	clReleaseMemObject(renderBuffer);
	clReleaseMemObject(cameraBuffer);
	clReleaseMemObject(spheresBuffer);

	// write to file
	writeToPPM("output.ppm", render.data(), renderWidth, renderHeight, 255);
}

int main(int argc, char** argv) {
	cl_int status;
	cl_device_id device;

	std::string scene;
	unsigned int width, height;
	unsigned int nSamples = 500; 
	unsigned int depth = 50;
	
	// validate command line args
	if (argc >= 4 && argc <= 6) {
		scene = argv[1];
		width = std::atoi(argv[2]);
		height = std::atoi(argv[3]);
		if (argc >= 5) {
			nSamples = std::atoi(argv[4]);
			depth = std::atoi(argv[5]);
		}
	} else {
		std::cout << "Invalid program use:" << std::endl;
		std::cout << "raytrace <scene path> <width> <height> [samples = 500][max depth = 50]" << std::endl;
		return -1;
	}

	width -= width % LOCAL_WG_SIZE_X;
	height -= height % LOCAL_WG_SIZE_Y;

	std::cout << "Initialising OpenCL" << std::endl;
	
	// set up OpenCL
	cl_context context = initialiseContext(&device);
	cl_command_queue queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);

	// create kernel programs
	cl_kernel kTraceSamples = compileKernelFromFile("./gpu/kernel.cl", "traceSamples", context, device);
	cl_kernel kAverageSamples = compileKernelFromFile("./gpu/kernel.cl", "averageSamples", context, device);
	KernelProgram kpRaytrace(context, queue, { {"traceSamples", kTraceSamples}, {"averageSamples", kAverageSamples} });

	std::cout << "Raytracing scene" << std::endl;
	std::vector<cl_Sphere> spheres;
	if (loadScene(spheres, scene) == -1)
		return -1;
	raytraceScene(kpRaytrace, spheres, width, height, nSamples, 20, depth);

	// free OpenCL resources
	clReleaseKernel(kTraceSamples);
	clReleaseKernel(kAverageSamples);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	clReleaseDevice(device);

	return 0;
}