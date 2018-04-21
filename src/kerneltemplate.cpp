#include "kerneltemplate.hpp"

void KernelTemplate::initKernel(cl::Context context, std::vector<cl::Device>::iterator deviceIter, 
	std::string fileName, std::string kernelName){

	// Creates command queue
	queue = cl::CommandQueue(context, *deviceIter);

	// Reads kernel function from file
	std::ifstream file(fileName);

	std::stringstream stream;
	stream << file.rdbuf();
	std::string str = stream.str();

	// Creates program object
	cl::Program::Sources sources(1, std::make_pair(str.c_str(), str.length()));
	program = cl::Program(context, sources);

	// Builds program
	program.build({ *deviceIter });

	// Creates kernel object
	kernel = cl::Kernel(program, kernelName.c_str());
}

std::string KernelTemplate::getProgramBuildInfo(std::vector<cl::Device>::iterator deviceIter){
	return program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(*deviceIter);
}

void KernelTemplate::setWorkSize(int NDRange, int numOfCUs, int maxWorkgroupSize, int wavefront){
	int ndRangeMul = (int)std::ceil((float)NDRange / wavefront);
	int workGroupMul = maxWorkgroupSize / wavefront;

	if (ndRangeMul <= numOfCUs) {
		globalWorkSize = ndRangeMul * wavefront;
		localWorkSize = wavefront;
	}
	else{
		int wavefrontPerCUs = (int)std::ceil(ndRangeMul / (float)numOfCUs);

		if (wavefrontPerCUs <= workGroupMul){
			localWorkSize = wavefrontPerCUs * wavefront;
			globalWorkSize = numOfCUs * localWorkSize;
		}
		else{
			int workGroupsPerCU = (int)std::ceil(wavefrontPerCUs / (float)workGroupMul);

			localWorkSize = (int)std::ceil(wavefrontPerCUs / (float)workGroupsPerCU) * wavefront;
			globalWorkSize = numOfCUs * workGroupsPerCU * localWorkSize;
		}
	}
}

void KernelTemplate::executeKernel(){
	queue.enqueueNDRangeKernel(kernel, cl::NDRange(), cl::NDRange(globalWorkSize), cl::NDRange(localWorkSize));
}