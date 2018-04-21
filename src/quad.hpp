#ifndef QUAD_H
#define QUAD_H

#include <vector>

#include "particle.hpp"

// Quad used in Barnes-Hut algorithm
class Quad{

	friend class BarnesHut;
	friend class BarnesHutOCL;

private:

	// Number of sub-quads
	int numOfQuads = 0;

	// Parent quad
	Quad *parent = nullptr;

	// Upper-left sub-quad
	Quad *nw = nullptr;
	// Upper-right sub-quad
	Quad *ne = nullptr;
	// Lower-left sub-quad
	Quad *sw = nullptr;
	// Lower-right sub-quad
	Quad *se = nullptr;

	// Contained particles indices
	std::vector<int> partIndexes;

	// Quad width
	float width;

	// Lower-left corner coordinates in regards to the parent quad
	float qx;
	float qy;

	// Quad mass
	float mass = 0;

	// Center of mass coordinates
	float cx = 0;
	float cy = 0;

	Quad(const Quad& q);
	~Quad();

	Quad& operator=(const Quad& q);

	// Initializes quad
	void initQuad(const Particles& p);

	// Checks if particle is in quad
	bool isPartInQuad(float x, float y, float w, float qx, float qy);

public:

	Quad(Quad *qp, const Particles& p, float w, float x, float y);

};

#endif