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

bool Main::isPrime(long num) {
	if (num <= 1) return false;
	if (num == 2) return true;
	if (num % 2 == 0) return false;
	long maxDivisor = static_cast<long>(ceil(sqrt(num)));

	for (long d = 3; d <= maxDivisor; d += 2) {
		if (num % d == 0) {
			return false;
		}
	}
	return true;
}

bool Main::isPrime(long num, long numThreads, bool immediatePrint, long& lastThread) {
	if (num <= 1) return false;
	if (num == 2) return true;
	if (num % 2 == 0) return false;

	bool isPrimeNum = true;
	long maxDivisor = static_cast<long>(ceil(sqrt(num)));
	long step = ceil((maxDivisor - 3) / numThreads);

	threads.clear();
	for (long i = 0; i < numThreads; ++i) {
		long start = 3 + 2 * i * step;
		long end = i == numThreads - 1 ? maxDivisor : (start + 2 * step - 1);

		if (end % 2 == 0) end--;
		if (start > end) continue;

		threads.emplace_back(&Main::checkDivisibility, this, i, start, end, num, ref(isPrimeNum), ref(lastThread));
	}

	for (auto& thread : threads) {
		thread.join();
	}

	return isPrimeNum;
}


void Main::checkDivisibility(long threadId, long start, long end, long num, bool& isPrimeNum, long& lastThread) {
	for (long i = start; i <= end; i += 2) {
		if (num % i == 0) {
			lock_guard<mutex> lock(mtx);
			isPrimeNum = false;
			return;
		}
	}

	lock_guard<mutex> lock(mtx);
	lastThread = threadId;
}

void Main::searchPrimes(long threadId, long start, long end, vector<long>& primes, bool immediatePrint) {
	for (long i = start; i <= end; i++) {
		if (isPrime(i)) {
			if (immediatePrint) {
				lock_guard<mutex> lock(mtx);
				getTimeStamp();
				cout << "Thread " << threadId << " found prime: " << i << " at " << getTimeStamp() << endl;
			}
			else {
				lock_guard<mutex> lock(mtx);
				primes.push_back(i);
			}
		}
	}
}

void Main::Run()
{
	bool immediatePrint = true;
	bool straightDivision = false;
	readConfig();

	long step = max / numThreads;
	
	if (straightDivision) {
		for (long i = 0; i < numThreads; ++i) {
			long start = i * step + 1;
			long end = (i == numThreads - 1) ? max : (i + 1) * step;
			threads.emplace_back(&Main::searchPrimes, this, i, start, end, ref(primes),immediatePrint);
		}

		for (auto& thread : threads) {
			thread.join();
		}
	}
	else {
		long lastThread = 0;

		for (long num = 2; num <= max; num++) {
			if (isPrime(num, numThreads, immediatePrint, ref(lastThread))) {
				if (immediatePrint) {
					cout << "Thread " << lastThread << " found prime: " << num << " at " << getTimeStamp() << endl;
				}
				else {
					primes.push_back(num);
				}
			}
		}
	}

	if (!immediatePrint) {
		cout << "Primes found: " << endl;
		for (long prime : primes) {
			cout << prime << endl;
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