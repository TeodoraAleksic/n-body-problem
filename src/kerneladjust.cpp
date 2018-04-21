#include "kerneladjust.hpp"

void KerneAdjust::setArguments(cl::Context context, int dt, int NDRange, 
	float* m, float* px, float* py, float* vx, float* vy, float* fx, float* fy){

	bm = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NDRange, m);
	bpx = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NDRange, px);
	bpy = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NDRange, py);
	bvx = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NDRange, vx);
	bvy = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NDRange, vy);
	bfx = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NDRange, fx);
	bfy = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NDRange, fy);

	kernel.setArg(0, dt);
	kernel.setArg(1, NDRange);
	kernel.setArg(2, bm);
	kernel.setArg(3, bpx);
	kernel.setArg(4, bpy);
	kernel.setArg(5, bvx);
	kernel.setArg(6, bvy);
	kernel.setArg(7, bfx);
	kernel.setArg(8, bfy);
}

void KerneAdjust::getResults(int NDRange, float* px, float* py, float* vx, float* vy){
	queue.enqueueReadBuffer(bpx, CL_TRUE, 0, sizeof(cl_float) * NDRange, px);
	queue.enqueueReadBuffer(bpy, CL_TRUE, 0, sizeof(cl_float) * NDRange, py);
	queue.enqueueReadBuffer(bvx, CL_TRUE, 0, sizeof(cl_float) * NDRange, vx);
	queue.enqueueReadBuffer(bvy, CL_TRUE, 0, sizeof(cl_float) * NDRange, vy);
}