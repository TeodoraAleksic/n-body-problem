#include "barneshut.hpp"

BarnesHut::BarnesHut(const Particles& p, int s) :NBodySim(p, s){
	simName = "Barnes-Hut algorithm";
	simFileName = "simbarneshut.csv";
}

void BarnesHut::runSimStep(){
	// Creates tree
	Quad *root = new Quad(nullptr, *this, width, 0, 0);

	std::vector<Quad*> buffer = std::vector<Quad*>();

	// Calculates gravitational forces
	for (int i = 0; i < numOfParticles; ++i){
		float px = positionX[i];
		float py = positionY[i];

		// Checks if particle is contained in root quad
		if (px >= 0 && px < width && py >= 0 && py < width){
			buffer.push_back(root);

			Quad *temp;

			// Visits each quad
			while (!buffer.empty()){
				// Takes not visited quad
				temp = buffer.back();
				buffer.pop_back();

				// Calculates distance between particle and quad
				float s = temp->width;
				float dx = positionX[i] - temp->cx;
				float dy = positionY[i] - temp->cy;
				float d = sqrt(dx * dx + dy * dy);

				// Checks if particle and quad are within the distance threshold
				if ((s / d) < theta){
					// Approximates force if threshold surpassed
					calcPartForce(i, temp->mass, temp->cx, temp->cy);
				}
				else{
					// Checks if quad contains more than one particle
					if (temp->partIndexes.size() > 1){
						// Puts sub-quads into queue
						if (temp->se != nullptr) buffer.push_back(temp->se);
						if (temp->sw != nullptr) buffer.push_back(temp->sw);
						if (temp->ne != nullptr) buffer.push_back(temp->ne);
						if (temp->nw != nullptr) buffer.push_back(temp->nw);
					}

					// Checks if quad contains at least one particle
					if (temp->partIndexes.size() == 1){
						int j = temp->partIndexes[0];

						if (i != j){
							// Calculates gravitational force between particles
							calcPartForce(i, mass[j], positionX[j], positionY[j]);
						}
					}
				}
			}
		}
	}

	delete root;

	// Adjusts particle data
	for (int i = 0; i < numOfParticles; ++i){
		adjustPartParams(i);
	}
}