#include "quad.hpp"

Quad::Quad(Quad *qp, const Particles& p, float w, float x, float y){
	parent = qp;

	width = w;
	qx = x;
	qy = y;

	partIndexes = std::vector<int>();

	initQuad(p);
}

Quad::~Quad(){
	delete nw;
	delete ne;
	delete sw;
	delete se;
}

void Quad::initQuad(const Particles& particles){
	// Parent quad number of particles
	int parentNumOfParticles;
	// Parent particles indices
	std::vector<int> parentIndexes = std::vector<int>();

	// Gets number of particles and particle indices if quad is not the tree root
	if (parent != nullptr){
		parentNumOfParticles = (int)parent->partIndexes.size();
		parentIndexes = parent->partIndexes;
	}
	else{
		parentNumOfParticles = particles.numOfParticles;
	}

	// Numbers of particles in each sub-quad
	int numNW = 0;
	int numNE = 0;
	int numSW = 0;
	int numSE = 0;

	// Searches parent particles and finds ones contained within the quad
	for (int i = 0; i < parentNumOfParticles; ++i){
		// Gets particle mass and position
		float m = (parent != nullptr) ? particles.mass[parentIndexes[i]]		: particles.mass[i];
		float x = (parent != nullptr) ? particles.positionX[parentIndexes[i]]	: particles.positionX[i];
		float y = (parent != nullptr) ? particles.positionY[parentIndexes[i]]	: particles.positionY[i];
		
		// Checks if particle in quad
		if (isPartInQuad(x, y, width, qx, qy)){
			// Saves particle index
			partIndexes.push_back((parent != nullptr) ? parentIndexes[i] : i);

			// Calculates quad mass and center of mass
			mass += m;
			cx += m * x;
			cy += m * y;

			// Increases sub-quad particle number if particle is contained within sub-quad
			numNW = isPartInQuad(x, y, width / 2, qx,				qy + width / 2) ? (numNW + 1) : numNW;
			numNE = isPartInQuad(x, y, width / 2, qx + width / 2,	qy + width / 2) ? (numNE + 1) : numNE;
			numSW = isPartInQuad(x, y, width / 2, qx,				qy)				? (numSW + 1) : numSW;
			numSE = isPartInQuad(x, y, width / 2, qx + width / 2,	qy)				? (numSE + 1) : numSE;
		}
	}

	// Calculates center of mass coordinates
	cx /= mass;
	cy /= mass;

	// Creates sub-quads if they contain particles
	if (partIndexes.size() > 1){
		nw = (numNW > 0) ? new Quad(this, particles, width / 2, qx,				qy + width / 2)	: nw;
		ne = (numNE > 0) ? new Quad(this, particles, width / 2, qx + width / 2, qy + width / 2) : ne;
		sw = (numSW > 0) ? new Quad(this, particles, width / 2, qx,				qy)				: sw;
		se = (numSE > 0) ? new Quad(this, particles, width / 2, qx + width / 2, qy)				: se;

		numOfQuads += (numNW > 0) ? (nw->numOfQuads) : 0;
		numOfQuads += (numNE > 0) ? (ne->numOfQuads) : 0;
		numOfQuads += (numSW > 0) ? (sw->numOfQuads) : 0;
		numOfQuads += (numSE > 0) ? (se->numOfQuads) : 0;
	}

	++numOfQuads;
}

bool Quad::isPartInQuad(float x, float y, float w, float qx, float qy){
	// Checks if particle is contained within quad
	if (((x >= qx) && (x < (qx + w))) && ((y >= qy) && (y < (qy + w)))){
		return true;
	}
	else{
		return false;
	}
}