std::string charToBits(char ch)
{
	std::string result;
	for (int i = 7; i >= 0; --i)
	{
		result += ((ch >> i) & 1) ? '1' : '0';
	}
	return result;
}

std::string stringToBits(const std::string &input)
{
	std::string result;
	for (size_t i = 0; i < input.size(); ++i)
	{
		result += charToBits(input[i]);
	}
	return result;
}

bool writeToFile(const std::string &filePath, const std::string &content)
{
	std::ofstream outputFile(filePath.c_str(), std::ios::binary);
	if (!outputFile.is_open())
	{
		std::cerr << "Error: Unable to open file: " << filePath << std::endl;
		return false;
	}

	// Convertir el contenido a bits
	std::string contentBits = stringToBits(content);

	// Escribir los bits en el archivo
	outputFile.write(contentBits.c_str(), contentBits.size());

	outputFile.close();
	return true;
}