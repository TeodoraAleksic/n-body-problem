#include "nbodysim.hpp"

NBodySim::NBodySim(const Particles& p, int s) :Particles(p){
	numOfSteps = s;
}

void NBodySim::calcPartForce(int i, float m, float x, float y){
	// Calculates distance between two particles
	float dx = positionX[i] - x;
	float dy = positionY[i] - y;
	float dist = sqrt(dx * dx + dy * dy);

	// Calculates the gravitational force between two particles
	float force = g * mass[i] * m * dist * (1.0f / pow(sqrt(dist * dist + eps * eps), 3));

	// Adjusts particle force vector
	forceX[i] += force * dx / dist;
	forceY[i] += force * dy / dist;
}

void NBodySim::adjustPartParams(int i){
	// Adjusts particle speed vector
	velocityX[i] += dt * forceX[i] / mass[i];
	velocityY[i] += dt * forceY[i] / mass[i];

	// Adjusts particle position vector
	positionX[i] += dt * velocityX[i];
	positionY[i] += dt * velocityY[i];

	// Resets particle force vector
	forceX[i] = 0;
	forceY[i] = 0;
}

void NBodySim::runSim(){
	std::cout << "Simulation\n";
	std::cout << "--------------------------------------------------\n";
	std::cout << simName << "\n\n";

	// Writes simulation results to file
	std::ofstream results;
	results.open(simFileName);

	if (!results.is_open()){
		std::cout << "ERROR::Failed to open file::" << simFileName << "\n\n";
		return;
	}

	results << "Iteration,Particle,Mass,PosX,PosY,VelX,VelY,\n";

	// Writes initial particle data
	for (int j = 0; j < numOfParticles; ++j){
		results << 0 << "," << j << "," << mass[j] << ","
			<< positionX[j] << "," << positionY[j] << ","
			<< velocityX[j] << "," << velocityY[j] << ",\n";
	}

	std::cout << "Running...\n\n";

	// Runs simulation numOfSteps times
	for (int i = 0; i < numOfSteps; ++i){
		// Runs one simulation step
		runSimStep();

		// Writes particle data
		for (int j = 0; j < numOfParticles; ++j){
			results << (i + 1) << "," << j << "," << mass[j] << ","
				<< positionX[j] << "," << positionY[j] << ","
				<< velocityX[j] << "," << velocityY[j] << ",\n";
		}
	}

	results.close();

	std::cout << "Finished simulation\n\n";
}