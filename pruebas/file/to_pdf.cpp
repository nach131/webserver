#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

int main(int argc, char **argv)
{
	// std::string command = "python3 /Users/nmota-bu/Desktop/test.py";
	// command.append(" > ./toma.txt");
	// int returnCode = std::system(command.c_str());
	// std::cout << returnCode << std::endl;
	std::ifstream in(".bin", std::ios::in | std::ios::binary);
	std::ofstream out("file.pdf", std::ios::out | std::ios::binary);
	if (!in.good())
	{
		std::cout << "In error" << std::endl;
		return 1;
	}
	if (!out.good())
	{
		std::cout << "Out error" << std::endl;
		return 1;
	}
	out << in.rdbuf();
	// std::copy(
	// 	std::istreambuf_iterator<char>(in),
	// 	std::istreambuf_iterator<char>(),
	// 	std::ostreambuf_iterator<char>(out));
	in.close();
	out.close();
	return 0;
}
