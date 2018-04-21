#include "particlegenerator.hpp"

const Particles& ParticleGenerator::getParticles(){
	return particles;
}

int ParticleGenerator::getNumOfParticles(){
	return particles.numOfParticles;
}

void ParticleGenerator::generateParticles(int numOfParticles, float width){
	srand((unsigned int)time(NULL));

	Particles temp;

	temp.numOfParticles = numOfParticles;
	temp.width = width;

	for (int i = 0; i < numOfParticles; ++i){
		if (i < 5){
			// Initializes 5 large particles
			temp.mass.push_back(bigMassMul * sunMass);
		}
		else{
			// Initializes the other particles
			temp.mass.push_back((rand() % (int)(100 * (maxMassMul - minMassMul)) + 100 * minMassMul) / 100 * sunMass);
		}

		// Initializes the position vectors of the particle
		temp.positionX.push_back((float)((rand() * rand()) % (int)width));
		temp.positionY.push_back((float)((rand() * rand()) % (int)width));

		// Initializes the speed vectors of the particles
		temp.velocityX.push_back((rand() % (int)(100 * (maxSpeedMul - minSpeedMul)) + 100 * minSpeedMul) / 100 * initSpeed);
		temp.velocityY.push_back((rand() % (int)(100 * (maxSpeedMul - minSpeedMul)) + 100 * minSpeedMul) / 100 * initSpeed);

		// Initializes the force vectors of the particles
		temp.forceX.push_back(0);
		temp.forceY.push_back(0);
	}

	particles = temp;
}

void ParticleGenerator::readPartFile(std::string fileName){
	std::ifstream file;
	file.open(fileName);

	if (!file.is_open()){
		std::cout << "ERROR::Failed to open file::" << fileName << "\n\n";
		return;
	}

	Particles temp;

	std::string line;
	std::string param;
	std::stringstream stream;

	// Reads number of bodies
	std::getline(file, line);
	stream = std::stringstream(line);

	std::getline(stream, param, ',');
	temp.numOfParticles = std::stoi(param);

	// Reads simulation space size
	std::getline(file, line);
	stream = std::stringstream(line);

	std::getline(stream, param, ',');
	temp.width = std::stof(param);

	// Reads particle data
	while (std::getline(file, line)){
		stream = std::stringstream(line);

		std::getline(stream, param, ',');
		temp.mass.push_back(std::stof(param));

		std::getline(stream, param, ',');
		temp.positionX.push_back(std::stof(param));

		std::getline(stream, param, ',');
		temp.positionY.push_back(std::stof(param));

		std::getline(stream, param, ',');
		temp.velocityX.push_back(std::stof(param));

		std::getline(stream, param, ',');
		temp.velocityY.push_back(std::stof(param));

		temp.forceX.push_back(0);
		temp.forceY.push_back(0);
	}

	particles = temp;
}

void ParticleGenerator::writePartFile(std::string fileName){
	std::ofstream file;
	file.open(fileName);

	if (!file.is_open()){
		std::cout << "ERROR::Failed to open file::" << fileName << "\n\n";
		return;
	}

	// Writes number of particles
	file << particles.numOfParticles << ",\n";
	// Writes simulation space size
	file << particles.width << ",\n";

	// Writes particle data
	for (int i = 0; i < particles.numOfParticles; ++i){
		file << particles.mass[i] << ","
			 << particles.positionX[i] << "," << particles.positionY[i] << ","
			 << particles.velocityX[i] << "," << particles.velocityY[i] << ",\n";
	}

	file.close();
}