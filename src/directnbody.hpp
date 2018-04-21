#ifndef DIRECT_H
#define DIRECT_H

#include "nbodysim.hpp"

// N-body simulation class
class DirectNBody: public NBodySim{
private:

	// Runs one simulation step
	void runSimStep();

public:

	DirectNBody(const Particles& p, int s=5);

};

#endif