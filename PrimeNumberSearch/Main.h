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
	bool isPrime(long num);
	bool isPrime(long num, long numThreads, bool immediatePrint, long& lastThread);
	void searchPrimes(long threadId, long start, long end, vector<long>& primes, bool immediatePrint);
	void checkDivisibility(long threadId, long start, long end, long num, bool& isPrimeNum, long& lastThread);

private:
	long numThreads;
	long max;
	vector<long> primes;
	vector<thread> threads;
	mutex mtx;
};