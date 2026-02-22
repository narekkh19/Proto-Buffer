// Proto Buffer.cpp : Defines the entry point for the application.
//

#include "Proto Buffer.h"


int main()
{
	const std::string filename = "example.txt";

	if (!std::filesystem::exists(filename)) {
		std::ofstream file("example.txt");
	}

	std::fstream file("example.txt", std::ios::in | std::ios::out);
	
	if (!file) {
		std::cout << "Failed to open file" << std::endl;
		std::exit(0);
	}

	file << "This file is created by me:" << std::endl
				<< "Person that wants to understand how visual studio handles everything:" << std::endl;
	
	file.seekg(0);
	std::string line;
	std::getline(file, line);
	std::cout << line;

	return 0;
}
