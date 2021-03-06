#include<iostream>
#include"../include/Incremental.h"
#include"../include/C11ThreadsVersion.h"
#include"../include/OpenMPVersion.h"
#include"../include/CUDAVersion.cuh"

#define VERTEXES 5000

const bool TEST_MODE = false;
const bool SAVE_MODE = false;
const std::string GRAPH_FILE = "gen/graph.txt";

void printVector(std::vector<unsigned> v) {
	std::cout << "isEmpty: " << v.empty() << std::endl;
	std::cout << "Vertexes: ";
	for (auto i = v.begin(); i != v.end(); ++i) std::cout << *i << ' ';
	std::cout << std::endl;
}

int incremental(std::string graphFilename, unsigned vertexesNumber) {
	Incremental* incremental = new Incremental(graphFilename, vertexesNumber);
	
	if (TEST_MODE) {
		incremental->printMatrix();
	}

	if (SAVE_MODE) {
		incremental->saveMatrix();
	}

	clock_t t = clock();
	Incremental::path* path = incremental->getCriticalPath();
	t = clock() - t;

	int len = path->pathLength;

	std::cout << "Incremental" << std::endl;
	std::cout << "pathLength: " << path->pathLength << std::endl;
	std::cout << "Calculated in: " << t << "[ms]\n" << std::endl;

	delete incremental;
	delete path;

	return len;
}

void parallelOpenMp(std::string graphFilename, unsigned vertexesNumber) {
	OpenMPVersion* openmp = new OpenMPVersion(graphFilename, vertexesNumber);

	if (TEST_MODE) {
		openmp->printMatrix();
	}

	if (SAVE_MODE) {
		openmp->saveMatrix();
	}

	clock_t t = clock();
	OpenMPVersion::path* path = openmp->getCriticalPath();
	t = clock() - t;

	std::cout << "OpenMP" << std::endl;
	std::cout << "pathLength: " << path->pathLength << std::endl;
	std::cout << "Calculated in: " << t << "[ms]\n" << std::endl;

	delete openmp;
	delete path;
}

void parallelCUDA(std::string graphFilename, unsigned vertexesNumber) {
	CUDAVersion* cuda = new CUDAVersion(graphFilename, vertexesNumber);

	if (TEST_MODE) {
		cuda->printMatrix();
	}

	if (SAVE_MODE) {
		cuda->saveMatrix();
	}

	clock_t t = clock();
	CUDAVersion::path* path = cuda->getCriticalPath();
	t = clock() - t;

	std::cout << "CUDA" << std::endl;
	std::cout << "Threads per block: " << cuda->getThreadsNumber() << std::endl;
	std::cout << "Blocks per grid: " << cuda->getBlocksNumber() << std::endl;
	std::cout << "pathLength: " << path->pathLength << std::endl;
	std::cout << "Calculated in: " << t << "[ms]" << std::endl;
	std::cout << "Kerneles calculated in: " << cuda->getMiliseconds() << "[ms]\n" << std::endl;
	
	delete cuda;
	delete path;
}

void parallelC11Threads(std::string graphFilename, unsigned vertexesNumber) {
	C11ThreadsVersion* threadsVersion = new C11ThreadsVersion(graphFilename, vertexesNumber);

	if (TEST_MODE) {
		threadsVersion->printMatrix();
	}

	if (SAVE_MODE) {
		threadsVersion->saveMatrix();
	}

	clock_t t = clock();
	C11ThreadsVersion::path* path = threadsVersion->getCriticalPath();
	t = clock() - t;

	std::cout << "C11Threads" << std::endl;
	std::cout << "pathLength: " << path->pathLength << std::endl;
	std::cout << "Calculated in: " << t << "[ms]\n" << std::endl;

	delete threadsVersion;
	delete path;
}


int main(int argc, char** argv) {

	if (argc > 1) {
		incremental(GRAPH_FILE, std::stoul(argv[1])); // default mode
	}
	else {
		std::cout << "VERTEXES: " << VERTEXES << std::endl;
		incremental(GRAPH_FILE, VERTEXES); 
		parallelOpenMp(GRAPH_FILE, VERTEXES);
		parallelC11Threads(GRAPH_FILE, VERTEXES); 
		parallelCUDA(GRAPH_FILE, VERTEXES);
	}
	std::cin.get();

}