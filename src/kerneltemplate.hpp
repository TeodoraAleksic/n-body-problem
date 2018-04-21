#ifndef KERNELTEMP_H
#define KERNELTEMP_H

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#define __CL_ENABLE_EXCEPTIONS

#include <iostream>
#include <fstream>
#include <sstream>

#include <CL\cl.h>
#include <CL\cl.hpp>

// Kernel base class
class KernelTemplate{
protected:

	// OpenCL command queue
	cl::CommandQueue queue;
	// OpenCL program
	cl::Program program;
	// OpenCL kernel object
	cl::Kernel kernel;

	// Global and local work size
	int globalWorkSize = 0;
	int localWorkSize = 0;

	KernelTemplate() {};

	// Builds program
	void initKernel(cl::Context context, std::vector<cl::Device>::iterator deviceIter,
		std::string fileName, std::string kernelName);

	// Gets program build info
	std::string getProgramBuildInfo(std::vector<cl::Device>::iterator deviceIter);

	// Sets work size
	void setWorkSize(int NDRange, int numOfCUs, int maxWorkgroupSize, int wavefront);

	// Executes kernel
	void executeKernel();

};

#endif