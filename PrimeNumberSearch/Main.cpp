#include "Main.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

Main::Main()
{
	numThreads = 0;
	min = 0;
	max = 0;
}

Main::~Main()
{

}

void Main::Run()
{
	string fileContent;
	string key;
	string value;
	ifstream ConfigFile("config.txt");

	while (getline(ConfigFile, fileContent)) {
		istringstream iss(fileContent);
		iss >> key >> value;
		if (key == "numThreads") {
			numThreads = stoi(value);
		}
		else if (key == "min") {
			min = stoi(value);
		}
		else if (key == "max") {
			max = stoi(value);
		}

		cout << key << " " << value << endl;
	}
}

int main()
{
	Main main;
	main.Run();
	return 0;
}