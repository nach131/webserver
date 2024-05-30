#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Nombre de fichero\n";
		return 1;
	}

	std::string fileName = argv[1];

	std::string command = "touch " + fileName;
	std::system(command.c_str());

	// std::system("touch img.png");
	// std::ifstream in("out", std::ios::in | std::ios::binary);
	// std::ofstream out("img.png", std::ios::out | std::ios::binary);

	std::ifstream in(".bin", std::ios::in | std::ios::binary);
	std::ofstream out(fileName, std::ios::out | std::ios::binary);
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
	// 	std::istreambuf_iterator<char>(in)
	// 	std::istreambuf_iterator<char>(),
	// 	std::ostreambuf_iterator<char>(out));
	in.close();
	out.close();

	std::system("rm .bin");

	return 0;
}
