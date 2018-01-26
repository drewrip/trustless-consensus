#include <mpi.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <chrono>
#include <fstream>
#include <sstream>
using namespace std;

const int endTime = 60 * 20; // 20 Minutes
const int startTime = 5; // 5 Seconds
const string hosts[6] = {"master", "node0", "node1", "node2", "node3", "node4"};
const string sendCommand = "$HOME/bitcoin/src/bitcoin-cli sendtoaddress 1F1tAaz5x1HUXrCNLbtMDqcw6o5GNn4xqX 0.000001";

std::string sysout(std::string command){
	std::string out;
	std::ostringstream resultStream;
	resultStream << command << " >> store.txt";
	std::string result = resultStream.str();
	system(result.c_str());
	std::ifstream readStream("store.txt");
	while(!readStream.eof()){
		readStream >> out; 
	}
	readStream.close();
	return out;
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

	ofstream data;
	data.open("bitcoin.csv");
	data << "Time,TXs\n";

	system("$HOME/bitcoin/src/bitcoind -regtest -daemon");

	system("$HOME/bitcoin/src/bitcoin-cli -regtest generate 101");

	MPI_Barrier(MPI_COMM_WORLD);

	for(int testTime = startTime; testTime <= endTime; testTime += 5){
		for(int i = 0; i < 3; i++){
			MPI_Barrier(MPI_COMM_WORLD);
			std::cout << "Working on " << testTime << "sec..." << std::endl;
			auto start = chrono::steady_clock::now();
			while(chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now()-start).count() < 5000){
				system(sendCommand.c_str());
			}
			MPI_Barrier(MPI_COMM_WORLD);
			if(worldRank == 0){
				system("$HOME/bitcoin/src/bitcoin-cli -regtest generate 1");
			}
			MPI_Barrier(MPI_COMM_WORLD);
		}
		if(worldRank == 0){
			std::ostringstream lastBlockHash;
			int maxBlocks = 0;
			stringstream blockString(sysout("$HOME/bitcoin/src/bitcoin-cli -regtest getblockcount"));
			blockString >> maxBlocks;
			std::ostringstream hashCommmand;
			hashCommmand << "$HOME/bitcoin/src/bitcoin-cli -regtest getblockhash " << maxBlocks - 4;
			std::string chunkCommand = hashCommmand.str();
			lastBlockHash << sysout(chunkCommand);
			std::string lastHash = lastBlockHash.str();
			std::ostringstream txStream;
			txStream << "$HOME/bitcoin/src/bitcoin-cli -regtest listsinceblock " << lastHash << " 1";
			std::string txCommand = txStream.str(); 
			std::string numTxs = sysout(txCommand);
			data << testTime << "," << numTxs << "\n";
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}



	// Cleans Up
	data.close();
	MPI_Finalize();
	return 0;
}