#include <iostream>
#include <stdlib.h>
#include <string>
#include <chrono>
#include <fstream>
#include <sstream>
#include <unistd.h>
using namespace std;

const int endTime = 60 * 10; // 10 Minutes
const int startTime = 10; // 5 Seconds
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

	ofstream data;
	data.open("bitcoin.csv");
	data << "Time,TXs\n";

	system("$HOME/bitcoin/src/bitcoind -regtest -daemon");
	sleep(2);
	system("$HOME/bitcoin/src/bitcoin-cli -regtest generate 101");

	for(int testTime = startTime; testTime <= endTime; testTime += 5){
		for(int i = 0; i < 3; i++){
			std::cout << "Working on " << testTime << "sec..." << std::endl;
			std::chrono::time_point<std::chrono::system_clock> end = chrono::system_clock::now() + (testTime * 1000);
			while(chrono::system_clock::now() < end){
				system(sendCommand.c_str());
			}

			system("$HOME/bitcoin/src/bitcoin-cli -regtest generate 1");

		}
		
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

	// Cleans Up
	data.close();
	return 0;
}