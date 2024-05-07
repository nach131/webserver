#include <iostream>
#include <vector>
#include <fstream>

int checkLine(std::string line)
{
	for (std::string::iterator it = line.begin(); it != line.end(); it++)
	{
		if (*it != 9 && *it != 0)
			return 0;
	}
	return 1;
}

int main(int argc, char **argv)
{
	std::string line;
	std::ifstream in(argv[1]);
	std::vector<std::string> arr;

	while (getline(in, line))
	{
		std::cout << "Line: -" << (int)(line[0]) << "- -" << line[0] << "-" << std::endl;
		if (line[0] == '\0' || checkLine(line))
			continue;
		arr.push_back(line);
	}
	in.close();
	for (std::vector<std::string>::iterator it = arr.begin(); it != arr.end(); it++)
	{
		std::cout << "Array line: " << *it << std::endl;
	}
}
