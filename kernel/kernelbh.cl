/*
	Kernel function which calculates the gravitational force between particles
*/

__kernel void kernelBH(__const float g, __const float eps, __const float theta, __const int numOfParticles, __const int numOfQuads,
	__global float *m, __global float *px, __global float *py, __global float *fx, __global float *fy,
	__global int *numOfSubQuads, __global int *particleIds, __global float *qs, __global float *qm, __global float *qx, __global float *qy){

	int id = get_global_id(0);

	if (id < numOfParticles){
		if (px[id] >= 0 && px[id] < qs[0] && py[id] >= 0 && py[id] < qs[0]){
			for (int i = 0; i < numOfQuads; ++i){
				bool calcForce = false;
				bool skipChildren = false;

				// Checks if quad is the tree root
				if (numOfSubQuads[i] == 1){
					if (id != particleIds[i]){
						calcForce = true;
					}
				}

				if (numOfSubQuads[i] > 1){
					// Calculates distance between particle and quad
					float s = qs[i];
					float dx = px[id] - qx[i];
					float dy = py[id] - qy[i];
					float d = sqrt(dx * dx + dy * dy);

					// Checks if particle and quad are within the distance threshold
					if ((s / d) < theta){
						calcForce = true;
						skipChildren = true;
					}
				}

				if (calcForce){
					// Calculates distance between two particles
					float dx = px[id] - qx[i];
					float dy = py[id] - qy[i];
					float dist = sqrt(dx * dx + dy * dy);

					// Calculates the gravitational force between two particles
					float force = g * m[id] * qm[i] * dist * (1.0f / pow(sqrt(dist * dist + eps * eps), 3));

					// Adjusts particle force vector
					fx[id] += force * dx / dist;
					fy[id] += force * dy / dist;
				}

				// Skips sub-quads if quad surpasses the distance threshold
				if (skipChildren){
					i += numOfSubQuads[i];
				}
			}
		}
	}
}