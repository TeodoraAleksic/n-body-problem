#ifndef NBODYSIM_H
#define NBODYSIM_H

#include <fstream>
#include <iostream>
#include <string>

#include "particle.hpp"

// N-body simulation base class
class NBodySim: public Particles{
protected:

	// Simulation name
	std::string simName;
	// Output file name
	std::string simFileName;

	// Number of simulation steps
	int numOfSteps;

	// Time interval
	int dt = 1;

	// Gravitational constant
	float g = 0.06f;
	// Gravitational softening length
	float eps = 1;
	// Length approximation threshold
	float theta = 0.5f;

	// Calculates force applied between two particles
	void calcPartForce(int i, float m, float x, float y);

	// Adjusts particle parameters
	void adjustPartParams(int i);

	// Runs one simulation step
	virtual void runSimStep() = 0;

public:

	NBodySim(const Particles& p, int s=5);

	// Runs simulation
	virtual void runSim();

};

#endif