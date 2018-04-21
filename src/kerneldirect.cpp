#include "kerneldirect.hpp"

void KernelDirect::setArguments(cl::Context context, float g, float eps, int NDRange, 
	float* m, float* px, float* py, float* fx, float* fy){

	bm = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NDRange, m);
	bpx = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NDRange, px);
	bpy = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NDRange, py);
	bfx = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NDRange, fx);
	bfy = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NDRange, fy);

	kernel.setArg(0, g);
	kernel.setArg(1, eps);
	kernel.setArg(2, NDRange);
	kernel.setArg(3, bm);
	kernel.setArg(4, bpx);
	kernel.setArg(5, bpy);
	kernel.setArg(6, bfx);
	kernel.setArg(7, bfy);
}

void KernelDirect::getResults(int NDRange, float* fx, float* fy){
	queue.enqueueReadBuffer(bfx, CL_TRUE, 0, sizeof(cl_float) * NDRange, fx);
	queue.enqueueReadBuffer(bfy, CL_TRUE, 0, sizeof(cl_float) * NDRange, fy);
}