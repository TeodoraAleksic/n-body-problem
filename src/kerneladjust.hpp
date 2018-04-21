#ifndef KERNELADJUST_H
#define KERNELADJUST_H

#include "kerneltemplate.hpp"

// Kernel class which adjusts particle data
class KerneAdjust : public KernelTemplate{

	friend class DirectNBodyOCL;
	friend class BarnesHutOCL;

private:

	cl::Buffer bm;
	cl::Buffer bpx;
	cl::Buffer bpy;
	cl::Buffer bvx;
	cl::Buffer bvy;
	cl::Buffer bfx;
	cl::Buffer bfy;

	KerneAdjust() {};

	// Sets kernel function arguments
	void setArguments(cl::Context context, int dt, int NDRange, 
		float* m, float* px, float* py, float* vx, float* vy, float* fx, float* fy);

	// Gets kernel function results
	void getResults(int NDRange, float* x, float* y, float* vx, float* vy);

};

#endif