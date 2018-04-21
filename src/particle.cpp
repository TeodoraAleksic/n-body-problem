#include "particle.hpp"

Particles::Particles(const Particles& p){
	copyParticles(p);
}

Particles& Particles::operator=(const Particles& p){
	if (this != &p) {
		copyParticles(p);
	}

	return *this;
}

void Particles::copyParticles(const Particles& p){
	numOfParticles = p.numOfParticles;
	width = p.width;

	mass = p.mass;

	positionX = p.positionX;
	positionY = p.positionY;

	velocityX = p.velocityX;
	velocityY = p.velocityY;

	forceX = p.forceX;
	forceY = p.forceY;
}