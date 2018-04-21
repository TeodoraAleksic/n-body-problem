#ifndef PARTGEN_H
#define PARTGEN_H

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

#include "particle.hpp"

// Particle generator class
class ParticleGenerator{
private:

	// Mass of the Sun
	float sunMass = 2;
	// Lowest particle mass multiplier
	float minMassMul = 0.5f;
	// Highest particle mass multiplier
	float maxMassMul = 4;
	// Large particle mass multiplier
	float bigMassMul = 20;

	// Initial particle speed
	float initSpeed = 0.01f;
	// Lowest particle speed multiplier
	float minSpeedMul = 1;
	// Highest particle speed multiplier
	float maxSpeedMul = 5;

	Particles particles;

public:

	ParticleGenerator() {};

	// Gets particles
	const Particles& getParticles();

	// Gets number of particles
	int getNumOfParticles();

	// Generates new set of particles
	void generateParticles(int numOfParticles=100, float width=10000);

	// Reads particle data from file
	void readPartFile(std::string fileName);

	// Writes particle data to file
	void writePartFile(std::string fileName);

};

#endif