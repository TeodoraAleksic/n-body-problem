#ifndef OPENCLPLATFORM_H
#define OPENCLPLATFORM_H

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#define __CL_ENABLE_EXCEPTIONS

#include <iostream>

#include <CL\cl.h>
#include <CL\cl.hpp>

// OpenCL platform and device class
class OpenCLPlatform{

	friend class DirectNBodyOCL;
	friend class BarnesHutOCL;

private:

	// Vendor name
	static const std::string vendors[];

	// Wavefront size
	int wavefront;

	// All OpenCL platforms in the system
	std::vector<cl::Platform> platforms;
	// Chosen OpenCL platform
	std::vector<cl::Platform>::iterator platformIter;

	// All OpenCL devices on the platform
	std::vector<cl::Device> devices;
	// Chosen OpenCL device
	std::vector<cl::Device>::iterator deviceIter;

	// OpenCL context
	cl::Context context;

	// Compute unit number
	int numOfCUs = 0;
	// Max workgroup size
	int maxWorkgroupSize = 0;

public:

	OpenCLPlatform() {};

	// Initializes AMD platform, device and context
	void initPlatform();

	// Prints OpenCL platform data
	void printPlatformSpecs();
	// Prints OpenCL device data
	void printDeviceSpecs();

};

#endif