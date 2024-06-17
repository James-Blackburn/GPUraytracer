#ifndef KERNEL_PROGRAM_HPP_INCLUDED
#define KERNEL_PROGRAM_HPP_INCLUDED

#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <CL/cl.h>

class KernelProgram {
private:
	cl_context context;
	cl_command_queue commandQueue;
	std::unordered_map<const char*, cl_kernel> kernels;
public:
	KernelProgram() = default;
	KernelProgram(cl_context& context_, cl_command_queue& queue_, std::unordered_map<const char*, cl_kernel> kernels_)
		: context(context_), commandQueue(queue_), kernels(kernels_) {};

	inline cl_context getContext();
	inline cl_command_queue getCommandQueue();
	inline cl_kernel getKernel(const char* kernelName);
};

cl_context KernelProgram::getContext() {
	return context;
}

cl_command_queue KernelProgram::getCommandQueue() {
	return commandQueue;
}

cl_kernel KernelProgram::getKernel(const char* kernelName) {
	try {
		return kernels.at(kernelName);
	} catch (std::out_of_range) {
		std::cerr << "kernel:" << kernelName << " not found" << std::endl;
		return nullptr;
	}
}

#endif