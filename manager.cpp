#include <mpi.h>
#include <iostream>
#include <stdlib.h>
#include <CompUtils.h>

using namespace std;


string InjectBlockTime::tspan = "consensus.nPowTargetTimespan =";
string InjectBlockTime::spacing = "consensus.nPowTargetSpacing =";

const int endTime = 60 * 20; // 20 Minutes
const int startTime = 5; // 5 Seconds
int msg;

void ready(int recvNode, int amtReadyNodes){
	int incoming = 0;
	while(incoming < amtReadyNodes){
		MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		incoming++;
	}
}

int main(){

	// Initialization Code for MPI specific utilities
	MPI_Init(NULL, NULL);

	// Stores the total number of processes in the network
	int worldSize;
	MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

	// Stores the rank(identification # starting at 0) of the current process
	int worldRank;
	MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);



	// Delete bitcoin folder in case old version is found
	system("rm -rf bitcoin");

	// Clone fresh bitcoin from fork
	system("git clone git://github.com/drewrip/bitcoin");

	for(int testTime = startTime; testTime <= endTime; testTime += 5){

		// Compiles bitcoin with new block time
		InjectBlockTime::inject(testTime);
		system("./bitcoin/autogen.sh && ./bitcoin/configure && make -j 2");

		
		if(worldRank == 0){// This node is dedicated to being the master, it will collect and manage data from the nodes

		}
		else if(worldRank == 1){ // This node is dedicated to being a miner, it will work to confirm blocks

		}
		else{ // This will catch the rest of the nodes and make them workers that continuously send transactions.

		}
	}



	// Cleans Up
	MPI_Finalize();
	return 0;
}
