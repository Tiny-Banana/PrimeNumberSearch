#include <mutex>
#include <vector>
#include <string>
#include <thread>

#pragma once
using namespace std;

class Main
{
public:
	Main();
	~Main();
	void Run();
	string getTimeStamp();
	void readConfig();
	bool isPrime(int num);
	bool isPrime(int num, int numThreads, bool immediatePrint);
	void searchPrimes(int threadId, int start, int end, vector<int>& primes, bool immediatePrint);
	void checkDivisibility(int threadId, int start, int end, int num, bool& isPrimeNum);

private:
	int numThreads;
	int max;
	vector<int> primes;
	vector<thread> threads;
	mutex mtx;
};