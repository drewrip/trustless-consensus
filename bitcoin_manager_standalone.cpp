#include <iostream>
#include <stdlib.h>
#include <string>
#include <chrono>
#include <fstream>
#include <sstream>
#include <unistd.h>

using namespace std;
using namespace std::chrono;

const int endTime = 60 * 10; // 10 Minutes
const int startTime = 10; // 10 Seconds
const string sendCommand = "$HOME/bitcoin/src/bitcoin-cli -regtest sendtoaddress miNcNbbqp39Th3QYkiqTXW2QauQ5JKcmjR 0.0001";
int txs = 0;

int main(){

	ofstream data;
	data.open("bitcoin.csv");
	data << "Time,TXs\n";
	system("$HOME/bitcoin/src/bitcoind -regtest -daemon");
	sleep(1);
	system("$HOME/bitcoin/src/bitcoin-cli -regtest generate 101");

	for(int testTime = startTime; testTime <= endTime; testTime += 10){
		for(int i = 0; i < 2; i++){
			std::cout << "Working on " << testTime << "sec... #" << i+1 << "/2"<< std::endl;
			std::chrono::milliseconds ms(testTime * 1000);
			std::chrono::time_point<std::chrono::steady_clock> end;
			end = std::chrono::steady_clock::now() + ms;
			while(std::chrono::steady_clock::now() < end){
				system(sendCommand.c_str());
				txs++;
			}
			system("$HOME/bitcoin/src/bitcoin-cli -regtest generate 1");

		}
		data << testTime << "," << txs << "\n";
		txs = 0;

	}

	// Cleans Up
	data.close();
	return 0;
}
