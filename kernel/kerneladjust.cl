/*
	Kernel function which adjusts particle data
*/

__kernel void kernelAdjust(__const int dt, __const int numOfParticles, 
	__global float *m, __global float *px, __global float *py,
	__global float *vx, __global float *vy, __global float *fx, __global float *fy){

	int id = get_global_id(0);

	if (id < numOfParticles){
		// Adjusts particle speed vector
		vx[id] += dt * fx[id] / m[id];
		vy[id] += dt * fy[id] / m[id];
		
		// Adjusts particle position vector
		px[id] += dt * vx[id];
		py[id] += dt * vy[id];

		// Resets particle force vector
		fx[id] = 0;
		fy[id] = 0;
	}
}