/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:48:00 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/25 22:02:31 by nmota-bu         ###   ########.fr       */
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
