#ifndef BH_H
#define BH_H

#include "nbodysim.hpp"
#include "quad.hpp"

// Barnes-Hut algorithm class
class BarnesHut: public NBodySim{

private:

	// Runs one simulation step
	void runSimStep();

public:

	BarnesHut(const Particles& p, int s = 5);

};

#endif