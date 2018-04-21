#ifndef DIRECTOCL_H
#define DIRECTOCL_H

#include <memory>

#include "openclplatform.hpp"
#include "kerneldirect.hpp"
#include "kerneladjust.hpp"
#include "nbodysim.hpp"

// N-body simulation class using OpenCL
class DirectNBodyOCL : public NBodySim{
private:

	// AMD platform
	OpenCLPlatform platform;

	// Kernel which calculates the gravitational force between particles
	KernelDirect kernelDirect;
	// Kernel which adjusts particle data
	KerneAdjust kernelAdjust;

	// OpenCL program error tags
	enum ProgramBuildError{None, Direct, Adjust};
	// Last failed program build
	ProgramBuildError buildError = None;

	// Prints OpenCL errors
	std::string errorCodeToString(cl_int error);

	// Runs one simulation step
	void runSimStep();

public:

	DirectNBodyOCL(const Particles& p, int s=5);

	// Runs simulation
	void runSim();

};


#endif