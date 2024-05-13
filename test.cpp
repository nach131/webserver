#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

int main(int argc, char **argv)
{
	std::string line = "Hola     que    tal";
	std::stringstream ss(line);
	std::string el;

	while (getline(el, ss))
	{
		std::cout << "Word: -" << el << "-" << std::endl;
	}
}
