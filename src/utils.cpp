/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:48:00 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/21 12:30:49 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

std::string DateField()
{
	// Obtener la fecha y hora actual
	time_t currentTime;
	time(&currentTime);
	struct tm *timeInfo = gmtime(&currentTime);

	// Formatear la fecha según el estándar RFC 1123 (ejemplo: "Tue, 15 Nov 1994 08:12:31 GMT")
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", timeInfo);
	return buffer;
}

std::string generateETag(const std::string &content)
{
	// Utiliza la función hash de la STL para generar un valor hash único
	std::hash<std::string> hasher;
	size_t hashValue = hasher(content);

	// Convierte el valor hash en una cadena hexadecimal
	std::stringstream ss;
	ss << std::hex << hashValue;
	return ss.str();
}

std::string readFile(const std::string &filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "Error al abrir el archivo " << filePath << std::endl;
		return ""; // Devuelve una cadena vacía si no se puede abrir el archivo
	}

	std::ostringstream oss;
	oss << file.rdbuf(); // Leer el contenido del archivo en un stringstream
	return oss.str();	 // Devolver el contenido como una cadena
}

std::string getExtension(const std::string &path)
{
	std::string extension;
	size_t pos = path.find_last_of('.');

	return path.substr(pos + 1);
}

//  // Retorna true si:
//     // 1. Se encontró un punto ('.') en la cadena (found != std::string::npos).
//     // 2. El punto no es el último carácter de la cadena (found != path.size() - 1).
bool isFile(const std::string &path)
{
	std::string adjustedPath = path;

	// Si el primer carácter es '.', eliminarlo
	if (!path.empty() && path[0] == '.')
		adjustedPath = path.substr(1);

	std::size_t found = adjustedPath.find_last_of(".");
	return (found != std::string::npos && found != path.size() - 1);
}
// #include <sys/stat.h>

// bool isFile(const std::string& path) {
//     struct stat statbuf;
//     if (stat(path.c_str(), &statbuf) != 0) {
//         return false; // No se pudo acceder a la ruta
//     }
//     return S_ISREG(statbuf.st_mode); // Verifica si es un archivo regular
// }

// Función para separar un string por un delimitador
std::vector<std::string> split(const std::string &str, char delimiter)
{
	std::vector<std::string> tokens;
	std::istringstream stream(str);
	std::string token;

	while (std::getline(stream, token, delimiter))
	{
		if (!token.empty())
			tokens.push_back(token);
	}

	return tokens;
}

bool isMainRoot(const std::string &str)
{
	// Contador para el número de slashes encontrados
	int slashCount = 0;

	// Recorrer cada carácter de la cadena
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
	{
		if (*it == '/')
			++slashCount;
	}

	// Verificar si hay exactamente un slash
	return (slashCount == 1);
}

std::string extractEndpoint(const std::string &url)
{
	// Encuentra la última ocurrencia de "/"
	size_t lastSlashPos = url.find_last_of("/");

	std::string endpoint;
	if (lastSlashPos != std::string::npos)
		// Usa substr para obtener la subcadena que sigue después de la última "/"
		endpoint = url.substr(lastSlashPos + 1);
	else
		// Si no se encuentra "/", devuelve la URL completa
		endpoint = url;

	// Elimina los caracteres de retorno de carro ("\r") del final de la cadena
	size_t pos = endpoint.find_last_not_of("\r");
	if (pos != std::string::npos)
		endpoint.erase(pos + 1);

	return endpoint;
}

void removeLastSlash(std::string &str)
{
	if (!str.empty() && str[str.length() - 1] == '/')
		str.erase(str.length() - 1);
}

std::string removeBeforeNumber(const std::string &url, const std::string &host)
{

	std::cout << "HOST: " << host << std::endl;

	std::string result = url;
	// std::size_t pos = result.find("8080"); // Buscar el número "8080"
	// TODO CUIDADO HARCODEADO
	// std::size_t pos = result.find("localhost:8080"); // Buscar el número "8080"
	std::size_t pos = result.find(host); // Buscar el número "8080"

	if (pos != std::string::npos)
	{
		// Encontrar la siguiente barra después del número
		pos = result.find("/", pos);
		if (pos != std::string::npos)
		{
			// Eliminar todo lo anterior e incluyendo la barra
			// result = result.substr(pos + 1);
			result = result.substr(pos);
		}
		else
			// Si no hay barra después del número, eliminar todo lo anterior e incluyendo el número
			result = "";
		// Eliminar cualquier carácter no deseado como '\r' al final de la cadena
		if (!result.empty() && result.back() == '\r')
			result.erase(result.size() - 1);
	}

	return result;
}

std::string removeFileName(const std::string &path)
{
	std::size_t pos = path.find_last_of('/');
	if (pos != std::string::npos)
		return path.substr(0, pos);
	return path; // Si no hay barra, devolver el path completo (significa que no hay directorios, solo un fichero)
}

void takeOutSemiColumn(std::string &val)
{
	size_t i = 0;
	size_t j = val.length() - 1;
	for (; i < val.length(); i++)
		if (val[i] == ';')
			break;
	for (; j >= i; j--)
		val.pop_back();
}

bool parseError(std::string str, int n)
{
	std::stringstream ss;
	ss << str << n;
	throw std::runtime_error(ss.str());
	return false;
}
