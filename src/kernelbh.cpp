#include "kernelbh.hpp"

void KernelBH::setArguments(cl::Context context, float g, float eps, float theta, int NDRange, int numOfQuads,
	float* m, float* px, float* py, float* fx, float* fy,
	int *numOfSubQuads, int *particleIds, float *qs, float *qm, float *qx, float *qy){

	bm = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NDRange, m);
	bpx = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NDRange, px);
	bpy = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NDRange, py);
	bfx = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NDRange, fx);
	bfy = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NDRange, fy);
	bnsq = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * NDRange, numOfSubQuads);
	bpid = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * NDRange, particleIds);
	bqs = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NDRange, qs);
	bqm = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NDRange, qm);
	bqx = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NDRange, qx);
	bqy = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NDRange, qy);

	kernel.setArg(0, g);
	kernel.setArg(1, eps);
	kernel.setArg(2, theta);
	kernel.setArg(3, NDRange);
	kernel.setArg(4, numOfQuads);
	kernel.setArg(5, bm);
	kernel.setArg(6, bpx);
	kernel.setArg(7, bpy);
	kernel.setArg(8, bfx);
	kernel.setArg(9, bfy);
	kernel.setArg(10, bnsq);
	kernel.setArg(11, bpid);
	kernel.setArg(12, bqs);
	kernel.setArg(13, bqm);
	kernel.setArg(14, bqx);
	kernel.setArg(15, bqy);
}

void KernelBH::getResults(int NDRange, float* fx, float* fy){
	queue.enqueueReadBuffer(bfx, CL_TRUE, 0, sizeof(cl_float) * NDRange, fx);
	queue.enqueueReadBuffer(bfy, CL_TRUE, 0, sizeof(cl_float) * NDRange, fy);
}