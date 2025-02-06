#include "Main.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;

Main::Main()
{
	numThreads = 0;
	max = 0;
}

Main::~Main()
{
}

bool Main::isPrime(int num) {
	if (num <= 1) return false;
	if (num == 2) return true;
	if (num % 2 == 0) return false;
	int maxDivisor = static_cast<int>(ceil(sqrt(num)));

	for (int d = 3; d <= maxDivisor; d += 2) {
		if (num % d == 0) {
			return false;
		}
	}
	return true;
}

bool Main::isPrime(int num, int numThreads, bool immediatePrint) {
	if (num <= 1) return false;
	if (num == 2) return true;
	if (num % 2 == 0) return false;

	bool isPrimeNum = true;
	int maxDivisor = static_cast<int>(ceil(sqrt(num)));

	//int step = std::max(1, static_cast<int>(ceil((maxDivisor - 3) / static_cast<double>(numThreads))));
	int step = ceil((maxDivisor - 3) / numThreads);

	threads.clear();

	for (int i = 0; i < numThreads; ++i) {
		int start = 3 + 2 * i * step;  
		int end = i == numThreads - 1 ? maxDivisor : (start + 2 * step - 1);		
	
		if (end % 2 == 0) end--; 
		if (start > end) continue;  

		cout << "ThreadID: " << i << endl;
		cout << "Start: " << start << endl;
		cout << "End: " << end << endl;

		threads.emplace_back(&Main::checkDivisibility, this, i, start, end, num, ref(isPrimeNum));
	}

	for (auto& thread : threads) {
		thread.join();
	}

	return isPrimeNum;
}

void Main::checkDivisibility(int threadId, int start, int end, int num, bool& isPrimeNum) {
	for (int i = start; i <= end; i += 2) {
		if (num % i == 0) {
			lock_guard<mutex> lock(mtx);
			isPrimeNum = false;
			return;
		}		
	}
}

void Main::searchPrimes(int threadId, int start, int end, vector<int>& primes, bool immediatePrint) {
	for (int i = start; i <= end; i++) {
		if (isPrime(i)) {
			if (immediatePrint) {
				lock_guard<mutex> lock(mtx);
				getTimeStamp();
				cout << "Thread " << threadId << " found prime: " << i << " at " << getTimeStamp()<< endl;
			}
			else {
				lock_guard<mutex> lock(mtx);
				primes.push_back(i);
			}
		}
	}
}

string Main::getTimeStamp() {
	auto now = chrono::system_clock::now();
	time_t now_time_t = chrono::system_clock::to_time_t(now);
	auto now_ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) % 1000;

	tm now_tm;
	localtime_s(&now_tm, &now_time_t);

	ostringstream oss;
	oss << put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << '.' << setfill('0') << setw(3) << now_ms.count();
	return oss.str();
}

void Main::Run()
{
	bool immediatePrint = false;
	bool straightDivision = false;
	readConfig();

	int step = max / numThreads;
	
	if (straightDivision) {
		for (int i = 0; i < numThreads; ++i) {
			int start = i * step + 1;
			int end = (i == numThreads - 1) ? max : (i + 1) * step;
			threads.emplace_back(&Main::searchPrimes, this, i, start, end, ref(primes),immediatePrint);
		}

		for (auto& thread : threads) {
			thread.join();
		}
	}
	else {
		for (int num = 2; num <= max; num++) {
			if (isPrime(num, numThreads, immediatePrint)) {
				primes.push_back(num);
				/*if (immediatePrint) {
					cout << "Found prime: " << num << endl;
				}
				else {
					
				}*/
			}
		}
	}

	if (!immediatePrint) {
		cout << "Primes found: " << endl;
		for (int prime : primes) {
			cout << prime << endl;
		}
	}
}

void Main::readConfig() {
	string fileContent;
	string key;
	string value;
	ifstream ConfigFile("config.txt");
		
	// input validation

	if (!ConfigFile.is_open()) {
		cerr << "Error: Could not open config.txt" << std::endl;
	}

	while (getline(ConfigFile, fileContent)) {
		istringstream iss(fileContent);
		iss >> key >> value;
		if (key == "x") {
			numThreads = stoi(value);
		}
		else if (key == "y") {
			max = stoi(value);
		}
	}

	if (numThreads > max) {
		cerr << "Error: Number of threads cannot be greater than the maximum number, setting x = y" << endl;
		numThreads = max;
	}
}

int main()
{
	Main main;
	cout << "Start: " << main.getTimeStamp() << endl;
	main.Run();
	cout << "End: " << main.getTimeStamp() << endl;
	return 0;
}