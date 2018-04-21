#include "directnbody.hpp"

DirectNBody::DirectNBody(const Particles& p, int s) :NBodySim(p, s){
	simName = "N-body simulation";
	simFileName = "simdirect.csv";
}

void DirectNBody::runSimStep(){
	// Calculates force applied between each pair of particles
	for (int i = 0; i < numOfParticles; ++i){
		for (int j = 0; j < numOfParticles; ++j){
			if (i != j){
				calcPartForce(i, mass[j], positionX[j], positionY[j]);
			}
		}
	}

	// Adjusts particle data
	for (int i = 0; i < numOfParticles; ++i){
		adjustPartParams(i);
	}
}