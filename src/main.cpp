#include "omp.h"

#include "barneshut.hpp"
#include "barneshutocl.hpp"
#include "directnbody.hpp"
#include "directnbodyocl.hpp"
#include "particlegenerator.hpp"

// Runs the n-body simulation
double directSim(const Particles& particles, int numOfSteps){
	DirectNBody directNBody(particles, numOfSteps);

	double start = omp_get_wtime();

	// Runs the simulation
	directNBody.runSim();

	double end = omp_get_wtime();

	return (end - start);
}

// Runs the Barnes-Hut algorithm
double bhSim(const Particles& particles, int numOfSteps){
	BarnesHut bh(particles, numOfSteps);

	double start = omp_get_wtime();

	// Runs the simulation
	bh.runSim();

	double end = omp_get_wtime();

	return (end - start);
}

// Runs the n-body simulation using OpenCL
double directOCLSim(const Particles& particles, int numOfSteps){
	DirectNBodyOCL directOCL(particles, numOfSteps);

	double start = omp_get_wtime();

	// Runs the simulation
	directOCL.runSim();

	double end = omp_get_wtime();

	return (end - start);
}

// Runs the Barnes-Hut algorithm using OpenCL
double bhOCLSim(const Particles& particles, int numOfSteps){
	BarnesHutOCL bhOCL(particles, numOfSteps);

	double start = omp_get_wtime();

	// Runs the simulation
	bhOCL.runSim();

	double end = omp_get_wtime();

	return (end - start);
}

// Saves the execution times
void saveResults(std::string fileName, int numOfParticles, int numOfSteps, 
				 double directTime, double bhTime, double directOCLTime, double bhOCLTime){
	std::ofstream results;
	results.open(fileName, std::ios_base::app);

	if (!results.is_open()){
		std::cout << "ERROR::Failed to open file::" << fileName << "\n\n";
		return;
	}

	results << "NumOfParticles,NumOfSteps,N-body,Barnes-Hut,N-body OpenCL,Barnes-Hut OpenCL\n";

	results << numOfParticles << "," << numOfSteps << "," 
		<< directTime << "," << bhTime << "," << directOCLTime << "," << bhOCLTime << ",\n";

	results.close();

	std::cout << "Written results to " << fileName << "\n";
}

int main(int argc, char** argv){
	
	std::string help = "Usage: nbodyproblem.exe (-i INPUT) [-o OUTPUT] [-s STEPS] [-h]\n\n"\
		               "Runs the direct N-body and Barnes-Hut simulations on the CPU and GPU using OpenCL.\n\n"\
					   "Options:\n"\
		               "-i INPUT   Input CSV file containing particle data. Required.\n"\
		               "-o OUTPUT  Output CSV file containing simulation results.\n"\
		               "-s STEPS   Number of simulation steps that will be executed. Default 1.\n"\
		               "-h         Prints usage.\n";

	std::string inputFile;
	std::string outputFile;
	int numOfSteps = 1;

	switch (argc)
	{
	case 1:
		// Prints help if no options were specified
		std::cout << help;
		return 0;
	case 2:
		// Prints help if -h option was specified
		if (argv[1] != std::string("-h"))
			std::cout << "ERROR::Invalid argument " << argv[1] << "\n";

		std::cout << help;
		return 0;
	case 3:
	case 5:
	case 7:
		// Parses the input parameters
		for (int i = 1; i < argc;){
			if (argv[i] == std::string("-i"))
				inputFile = argv[i + 1];
			else if (argv[i] == std::string("-o"))
				outputFile = argv[i + 1];
			else if (argv[i] == std::string("-s"))
				numOfSteps = atoi(argv[i + 1]);
			else{
				std::cout << "ERROR::Invalid argument " << argv[1] << "\n";
				std::cout << help;
				return 0;
			}

			i += 2;
		}

		// Checks if input file has been specified
		if (inputFile.empty()){
			std::cout << "ERROR::Missing required argument -i\n";
			std::cout << help;
			return 0;
		}

		break;
	default:
		// Prints help if invalid number of options was specified
		std::cout << "ERROR::Invalid number of parameters\n";
		std::cout << help;
		return 0;
	}

	ParticleGenerator partGen;

	// Reads particle data from file
	partGen.readPartFile(inputFile);

	int numOfParticles = partGen.getNumOfParticles();

	// Runs the n-body simulation
	double directTime = directSim(partGen.getParticles(), numOfSteps);
	
	// Runs the Barnes-Hut algorithm
	double bhTime = bhSim(partGen.getParticles(), numOfSteps);

	// Runs the n-body simulation using OpenCL
	double directOCLTime = directOCLSim(partGen.getParticles(), numOfSteps);

	// Runs the Barnes-Hut algorithm using OpenCL
	double bhOCLTime = bhOCLSim(partGen.getParticles(), numOfSteps);

	// Prints results
	std::cout << "Results\n";
	std::cout << "--------------------------------------------------\n";
	std::cout << "N-body:			: "	<< directTime		<< "\n";
	std::cout << "Barnes-Hut		: "	<< bhTime			<< "\n";
	std::cout << "N-body OpenCL		: "	<< directOCLTime	<< "\n";
	std::cout << "Barnes-Hut OpenCL	: "	<< bhOCLTime		<< "\n";
	std::cout << "\n";

	// Saves the execution times
	if (!outputFile.empty())
		saveResults(outputFile, numOfParticles, numOfSteps, directTime, bhTime, directOCLTime, bhOCLTime);
}