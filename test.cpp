#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

int main(int argc, char **argv)
{
	std::string line = "Hola     que    tal";
	std::stringstream ss(line);
	std::string key, value;

	while (ss >> key >> value)
	{
		std::cout << "Key: -" << key << "- -" << value << "-" << std::endl;
	}
}
