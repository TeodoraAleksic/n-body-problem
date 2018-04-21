#ifndef BHOCL_H
#define BHOCL_H

#include <memory>

#include "openclplatform.hpp"
#include "kernelbh.hpp"
#include "kerneladjust.hpp"
#include "nbodysim.hpp"
#include "quad.hpp"

// Barnes-Hut algorithm class using OpenCL
class BarnesHutOCL : public NBodySim{
private:

	// AMD platform
	OpenCLPlatform platform;

	// Kernel which calculates the gravitational force between particles
	KernelBH kernelBH;
	// Kernel which adjusts particle data
	KerneAdjust kernelAdjust;

	// OpenCL program error tags
	enum ProgramBuildError{ None, BH, Adjust };
	// Last failed program build
	ProgramBuildError buildError = None;

	// Prints OpenCL errors
	std::string errorCodeToString(cl_int error);

	// Runs one simulation step
	void runSimStep();

public:

	BarnesHutOCL(const Particles& p, int s=5);

	// Runs simulation
	void runSim();

};

#endif