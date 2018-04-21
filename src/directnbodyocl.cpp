#include "directnbodyocl.hpp"

DirectNBodyOCL::DirectNBodyOCL(const Particles& p, int s) :NBodySim(p, s){
	simName = "N-body simulation using OpenCL";
	simFileName = "simdirectocl.csv";
}

std::string DirectNBodyOCL::errorCodeToString(cl_int error){
	switch (error){
	case 0: return "CL_SUCCESS";
	case -1: return "CL_DEVICE_NOT_FOUND";
	case -2: return "CL_DEVICE_NOT_AVAILABLE";
	case -3: return "CL_COMPILER_NOT_AVAILABLE";
	case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
	case -5: return "CL_OUT_OF_RESOURCES";
	case -6: return "CL_OUT_OF_HOST_MEMORY";
	case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
	case -8: return "CL_MEM_COPY_OVERLAP";
	case -9: return "CL_IMAGE_FORMAT_MISMATCH";
	case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
	case -11: return "CL_BUILD_PROGRAM_FAILURE";
	case -12: return "CL_MAP_FAILURE";
	case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
	case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
	case -15: return "CL_COMPILE_PROGRAM_FAILURE";
	case -16: return "CL_LINKER_NOT_AVAILABLE";
	case -17: return "CL_LINK_PROGRAM_FAILURE";
	case -18: return "CL_DEVICE_PARTITION_FAILED";
	case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";
	case -30: return "CL_INVALID_VALUE";
	case -31: return "CL_INVALID_DEVICE_TYPE";
	case -32: return "CL_INVALID_PLATFORM";
	case -33: return "CL_INVALID_DEVICE";
	case -34: return "CL_INVALID_CONTEXT";
	case -35: return "CL_INVALID_QUEUE_PROPERTIES";
	case -36: return "CL_INVALID_COMMAND_QUEUE";
	case -37: return "CL_INVALID_HOST_PTR";
	case -38: return "CL_INVALID_MEM_OBJECT";
	case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
	case -40: return "CL_INVALID_IMAGE_SIZE";
	case -41: return "CL_INVALID_SAMPLER";
	case -42: return "CL_INVALID_BINARY";
	case -43: return "CL_INVALID_BUILD_OPTIONS";
	case -44: return "CL_INVALID_PROGRAM";
	case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
	case -46: return "CL_INVALID_KERNEL_NAME";
	case -47: return "CL_INVALID_KERNEL_DEFINITION";
	case -48: return "CL_INVALID_KERNEL";
	case -49: return "CL_INVALID_ARG_INDEX";
	case -50: return "CL_INVALID_ARG_VALUE";
	case -51: return "CL_INVALID_ARG_SIZE";
	case -52: return "CL_INVALID_KERNEL_ARGS";
	case -53: return "CL_INVALID_WORK_DIMENSION";
	case -54: return "CL_INVALID_WORK_GROUP_SIZE";
	case -55: return "CL_INVALID_WORK_ITEM_SIZE";
	case -56: return "CL_INVALID_GLOBAL_OFFSET";
	case -57: return "CL_INVALID_EVENT_WAIT_LIST";
	case -58: return "CL_INVALID_EVENT";
	case -59: return "CL_INVALID_OPERATION";
	case -60: return "CL_INVALID_GL_OBJECT";
	case -61: return "CL_INVALID_BUFFER_SIZE";
	case -62: return "CL_INVALID_MIP_LEVEL";
	case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
	case -64: return "CL_INVALID_PROPERTY";
	case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
	case -66: return "CL_INVALID_COMPILER_OPTIONS";
	case -67: return "CL_INVALID_LINKER_OPTIONS";
	case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";
	case -69: return "CL_INVALID_PIPE_SIZE";
	case -70: return "CL_INVALID_DEVICE_QUEUE";
	default: return "Unknown OpenCL error";
	}
}

void DirectNBodyOCL::runSimStep(){
	// Copies kernel argument data to device
	kernelDirect.setArguments(platform.context, g, eps, numOfParticles, 
		mass.data(), positionX.data(), positionY.data(), forceX.data(), forceY.data());

	// Runs kernel function
	kernelDirect.executeKernel();

	// Copies kernel results from device
	kernelDirect.getResults(numOfParticles, forceX.data(), forceY.data());

	// Copies kernel argument data to device
	kernelAdjust.setArguments(platform.context, dt, numOfParticles, 
		mass.data(), positionX.data(), positionY.data(), velocityX.data(), velocityY.data(), forceX.data(), forceY.data());

	// Runs kernel function
	kernelAdjust.executeKernel();

	// Copies kernel results from device
	kernelAdjust.getResults(numOfParticles, positionX.data(), positionY.data(), velocityX.data(), velocityY.data());
}

void DirectNBodyOCL::runSim(){
	try{
		std::cout << "Simulation\n";
		std::cout << "--------------------------------------------------\n";
		std::cout << simName << "\n\n";

		// Initializes AMD platform and device
		platform.initPlatform();

		// Prints platform and device specs
		platform.printPlatformSpecs();
		platform.printDeviceSpecs();

		buildError = Direct;

		// Creates kernel object
		kernelDirect.initKernel(platform.context, platform.deviceIter, "kernel/kerneldirect.cl", "kernelDirect");

		// Initializes kernel arguments
		kernelDirect.setWorkSize(numOfParticles, platform.numOfCUs, platform.maxWorkgroupSize, platform.wavefront);

		buildError = Adjust;

		// Creates kernel object
		kernelAdjust.initKernel(platform.context, platform.deviceIter, "kernel/kerneladjust.cl", "kernelAdjust");

		// Initializes kernel arguments
		kernelAdjust.setWorkSize(numOfParticles, platform.numOfCUs, platform.maxWorkgroupSize, platform.wavefront);

		// Writes simulation results to file
		std::ofstream results;
		results.open(simFileName);

		if (!results.is_open()){
			std::cout << "ERROR::Failed to open file::" << simFileName << "\n\n";
			return;
		}

		results << "Iteration,Particle,Mass,PosX,PosY,VelX,VelY,\n";

		// Writes initial particle data
		for (int j = 0; j < numOfParticles; ++j){
			results << 0 << "," << j << "," << mass[j] << ","
				<< positionX[j] << "," << positionY[j] << ","
				<< velocityX[j] << "," << velocityY[j] << ",\n";
		}

		std::cout << "Running...\n\n";

		// Runs simulation numOfSteps times
		for (int i = 0; i < numOfSteps; ++i){
			// Runs one simulation step
			runSimStep();

			// Writes particle data
			for (int j = 0; j < numOfParticles; ++j){
				results << (i + 1) << "," << j << "," << mass[j] << ","
					<< positionX[j] << "," << positionY[j] << ","
					<< velocityX[j] << "," << velocityY[j] << ",\n";
			}
		}

		results.close();

		std::cout << "Finished simulation\n\n";
	}
	catch (cl::Error error){
		std::cout << "ERROR::";

		// Prints OpenCL error
		std::cout << errorCodeToString(error.err()) << "::" << error.what() << "\n\n";

		// Prints build errors
		if (error.err() == CL_BUILD_PROGRAM_FAILURE){
			// Prints KernelDirect build errors
			if (buildError == Direct){
				std::string strDirect = kernelDirect.getProgramBuildInfo(platform.deviceIter);

				std::cout << "KernelDirect Build Info::" << strDirect << "\n";
			}

			// Prints KernelAdjust build errors
			if (buildError == Adjust){
				std::string strAdjust = kernelAdjust.getProgramBuildInfo(platform.deviceIter);

				std::cout << "KernelAdjust Build Info::" << strAdjust << "\n";
			}
		}
	}
	catch (std::string str){
		std::cout << "ERROR::" << str << "\n\n";
	}
}