#ifndef KERNELBH_H
#define KERNELBH_H

#include "kerneltemplate.hpp"

// Kernel class which calculates the gravitational force between particles
class KernelBH : public KernelTemplate{

	friend class BarnesHutOCL;

private:

	cl::Buffer bm;
	cl::Buffer bpx;
	cl::Buffer bpy;
	cl::Buffer bfx;
	cl::Buffer bfy;
	cl::Buffer bnsq;
	cl::Buffer bpid;
	cl::Buffer bqs;
	cl::Buffer bqm;
	cl::Buffer bqx;
	cl::Buffer bqy;

	KernelBH() {};

	// Sets kernel function arguments
	void setArguments(cl::Context context, float g, float eps, float theta, int NDRange, int numOfQuads,
		float* m, float* px, float* py, float* fx, float* fy,
		int *numOfSubQuads, int *particleIds, float *qs, float *qm, float *qx, float *qy);

	// Gets kernel function results
	void getResults(int NDRange, float* fx, float* fy);

};

#endif