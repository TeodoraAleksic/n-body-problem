#include "openclplatform.hpp"

const std::string OpenCLPlatform::vendors[3] = { "NVIDIA Corporation", "Advanced Micro Devices, Inc.", "Intel(R) Corporation" };

void OpenCLPlatform::initPlatform(){
	// Gets all OpenCL platforms
	cl::Platform::get(&platforms);

	int i;

	// Finds platform
	for (platformIter = platforms.begin(); platformIter != platforms.end(); ++platformIter){
		for (i = 0; i < 3; ++i) {
			if (strcmp(platformIter->getInfo<CL_PLATFORM_VENDOR>().c_str(), vendors[i].c_str()) == 0) {
				break;
			}
		}

		if (i != 3) break;
	}

	if (i == 3){
		throw std::string("Failed to find OpenCL platform\n");
	}
	
	switch (i){
	case 0:
		wavefront = 32;
		break;
	case 1:
	case 2:
		wavefront = 64;
		break;
	}

	// Gets all OpenCL GPU devices
	platformIter->getDevices(CL_DEVICE_TYPE_GPU, &devices);

	// Finds device
	for (deviceIter = devices.begin(); deviceIter != devices.end(); ++deviceIter){
		if (strcmp(deviceIter->getInfo<CL_DEVICE_VENDOR>().c_str(), vendors[i].c_str()) == 0){
			break;
		}
	}

	if (deviceIter == devices.end()){
		throw std::string("Failed to find OpenCL device\n");
	}

	// Gets device parameters
	numOfCUs = deviceIter->getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
	maxWorkgroupSize = (int)deviceIter->getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();

	// Creates OpenCL context
	context = cl::Context(*deviceIter);
}

void OpenCLPlatform::printPlatformSpecs(){
	// Prints OpenCL platform data
	if (platformIter != platforms.end()){
		std::cout << "Platform\n";
		std::cout << "Name:		"		<< platformIter->getInfo<CL_PLATFORM_NAME>()	<< "\n";
		std::cout << "Vendor:		"	<< platformIter->getInfo<CL_PLATFORM_VENDOR>()	<< "\n";
		std::cout << "Version:	"		<< platformIter->getInfo<CL_PLATFORM_VERSION>() << "\n";
		std::cout << "\n";
	}
	else{
		std::cout << "ERROR::Failed to find OpenCL platform\n\n";
	}
}

void OpenCLPlatform::printDeviceSpecs(){
	// Prints OpenCL device data
	if (deviceIter != devices.end()){
		std::cout << "Device\n";
		std::cout << "Name:		"		<< deviceIter->getInfo<CL_DEVICE_NAME>()	<< "\n";
		std::cout << "vendor:		"	<< deviceIter->getInfo<CL_DEVICE_VENDOR>()	<< "\n";
		std::cout << "Version:	"		<< deviceIter->getInfo<CL_DEVICE_VERSION>()	<< "\n";
		std::cout << "\n";
	}
	else{
		std::cout << "ERROR::Failed to find OpenCL device\n\n";
	}
}