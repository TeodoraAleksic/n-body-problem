/*
	Kernel function which calculates the gravitational force between particles
*/

__kernel void kernelDirect(__const float g, __const float eps, __const int numOfParticles, 
	__global float *m, __global float *px, __global float *py, __global float *fx, __global float *fy){

	int id = get_global_id(0);

	if (id < numOfParticles){
		for (int i = 0; i < numOfParticles; ++i){
			if (id != i){
				// Calculates distance between two particles
				float dx = px[id] - px[i];
				float dy = py[id] - py[i];
				float dist = sqrt(dx * dx + dy * dy);

				// Calculates the gravitational force between two particles
				float force = g * m[id] * m[i] * dist * (1.0f / pow(sqrt(dist * dist + eps * eps), 3));

				// Adjusts particle force vector
				fx[id] += force * dx / dist;
				fy[id] += force * dy / dist;
			}
		}
	}
}