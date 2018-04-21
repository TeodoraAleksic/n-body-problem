#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>

// Particle class
class Particles{

	friend class ParticleGenerator;
	friend class Quad;

protected:

	// Number of particles
	int numOfParticles = 0;

	// Simulation space width
	float width = 0;

	// Particle mass
	std::vector<float> mass = std::vector<float>();

	// Particle position vectors
	std::vector<float> positionX = std::vector<float>();
	std::vector<float> positionY = std::vector<float>();

	// Particle speed vectors
	std::vector<float> velocityX = std::vector<float>();
	std::vector<float> velocityY = std::vector<float>();

	//  Particle force vectors
	std::vector<float> forceX = std::vector<float>();
	std::vector<float> forceY = std::vector<float>();

	Particles() {};
	~Particles() {};

	Particles(const Particles& p);
	Particles& operator=(const Particles& p);

	// Copies particle data
	void copyParticles(const Particles& particles);

};

#endif