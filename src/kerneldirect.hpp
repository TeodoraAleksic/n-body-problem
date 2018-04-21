#ifndef KERNELDIRECT_H
#define KERNELDIRECT_H

#include "kerneltemplate.hpp"

// Kernel class which calculates the gravitational force between particles
class KernelDirect: public KernelTemplate{

	friend class DirectNBodyOCL;

private:

	cl::Buffer bm;
	cl::Buffer bpx;
	cl::Buffer bpy;
	cl::Buffer bfx;
	cl::Buffer bfy;

	KernelDirect() {};

	// Sets kernel function arguments
	void setArguments(cl::Context context, float g, float eps, int NDRange, 
		float* m, float* px, float* py, float* fx, float* fy);

	// Gets kernel function results
	void getResults(int NDRange, float* fx, float* fy);

};

#endif