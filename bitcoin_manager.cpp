#include <mpi.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <chrono>
using namespace std;

const int endTime = 60 * 20; // 20 Minutes
const int startTime = 5; // 5 Seconds
const string hosts[6] = {"master", "node0", "node1", "node2", "node3", "node4"};

int main(){

	// Initialization Code for MPI specific utilities
	MPI_Init(NULL, NULL);

	// Stores the total number of processes in the network
	int worldSize;
	MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

	// Stores the rank(identification # starting at 0) of the current process
	int worldRank;
	MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

	system("$HOME/bitcoin/src/bitcoind -regtest -daemon");
	for(string name : hosts){
		system("$HOME/bitcoin/src/bitcoin-cli -regtest addnode " + name + " \"add\"");
	}
	system("$HOME/bitcoin/src/bitcoin-cli -regtest generate 101")

	MPI_Barrier(MPI_COMM_WORLD);

	for(int testTime = startTime; testTime <= endTime; testTime += 5){

		MPI_Barrier(MPI_COMM_WORLD);
	}



	// Cleans Up
	MPI_Finalize();
	return 0;
}