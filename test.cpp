#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

int main(int argc, char **argv)
{
	std::string command = "python3 /Users/nmota-bu/Desktop/test.py";
	command.append(" > ./toma.txt");
	int returnCode = std::system(command.c_str());
	std::cout << returnCode << std::endl;
}
