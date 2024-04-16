#include <iostream>
#include <map>

class HTTPError
{
private:
	static std::map<int, std::string> errorMessages;

public:
	static void initialize()
	{
		errorMessages[400] = "Bad Request";
		errorMessages[401] = "Unauthorized";
		errorMessages[403] = "Forbidden";
		errorMessages[404] = "Not Found";
		errorMessages[500] = "Internal Server Error";
	}

	static std::string getMessage(int code)
	{
		std::map<int, std::string>::iterator it = errorMessages.find(code);
		if (it != errorMessages.end())
			return it->second;
		else
			return "Unknown Error";
	}
};

std::map<int, std::string> HTTPError::errorMessages;

int main()
{
	// Inicializar la clase HTTPError
	HTTPError::initialize();

	// Ejemplo de uso: obtener mensaje de error para el código 404
	std::cout << "Mensaje de error para el código 404: " << HTTPError::getMessage(404) << std::endl;

	// Ejemplo de uso: obtener mensaje de error para un código no existente
	std::cout << "Mensaje de error para un código no existente: " << HTTPError::getMessage(503) << std::endl;

	return 0;
}
