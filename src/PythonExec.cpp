/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PythonExec.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 10:02:31 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/28 10:22:43 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PythonExec.hpp"
#include <cstdlib>
#include <fstream>
#include <sstream>

void sleep_ms(int milliseconds)
{
	struct timespec ts;
	ts.tv_sec = milliseconds / 1000;
	ts.tv_nsec = (milliseconds % 1000) * 1000000;
	nanosleep(&ts, NULL);
}

std::string execPython(const std::string &py)
{
	std::string result;

	std::cout << YELLOW;
	std::cout << "execPython ONE\n ";
	std::cout << "py: " << py << std::endl;
	std::cout << RESET;

	// Construir el comando del sistema
	std::string command = "python3 " + py + " > ./conf_web/.tmp";
	int returnCode = std::system(command.c_str());

	// Verificar el éxito del comando del sistema
	if (returnCode != 0)
		throw std::system_error(returnCode, std::generic_category(), "Error al ejecutar el comando del sistema");

	// Leer el archivo temporal
	std::ifstream tempFile("./conf_web/.tmp");
	if (!tempFile.good())
		throw std::runtime_error("Error: No se pudo abrir el archivo temporal para leer");

	std::stringstream ss;
	ss << tempFile.rdbuf();
	result = ss.str();

	return result;
}

std::string execPython(const std::string &py, const std::string &data)
{
	std::string result;

	std::cout << YELLOW;

	std::cout << "execPython DOS\n ";
	std::cout << " py" << py << std::endl;
	std::cout << " data: " << data << RESET << std::endl;

	// Construir el comando del sistema
	std::string command = "python3 " + py + " " + data;
	int returnCode = std::system(command.c_str());

	// Verificar el éxito del comando del sistema
	if (returnCode != 0)
		throw std::system_error(returnCode, std::generic_category(), "Error al ejecutar el comando del sistema");

	// Esperar a que el archivo temporal sea creado y escrito
	std::ifstream tempFile;
	int attempts = 10; // Número máximo de intentos para comprobar el archivo
	while (attempts-- > 0)
	{
		tempFile.open("./conf_web/.output.html");
		if (tempFile.good())
			break;
		tempFile.close();
		sleep_ms(100); // Esperar 100 ms antes de execPython
	}

	if (!tempFile.good())
		throw std::runtime_error("Error: No se pudo abrir el archivo temporal para leer");

	// Leer el archivo temporal
	std::stringstream ss;
	ss << tempFile.rdbuf();
	result = ss.str();
	tempFile.close();

	// Limpiar el archivo temporal
	if (std::remove("./conf_web/.output.html") != 0)
		std::cerr << "Advertencia: No se pudo eliminar el archivo temporal" << std::endl;

	return result;
}

std::string execPython(const std::string &py, const std::string &dirPath, const std::string &root_location)
{
	std::string result;

	std::cout << YELLOW;
	std::cout << "execPython TRES\n";
	std::cout << " py: " << py << std::endl;
	std::cout << " dirPath: " << dirPath << std::endl;
	std::cout << " root_location: " << root_location << std::endl;
	std::cout << RESET;

	// Verificar la existencia del script de Python
	// if (!std::ifstream(py).good())
	// 	throw std::runtime_error("Error: No se encontró el archivo del script de Python: " + py);

	// // Verificar la existencia del directorio de destino
	// if (!std::ifstream(dirPath).good())
	// 	throw std::runtime_error("Error: No se encontró el directorio especificado: " + dirPath);

	// Construir el comando del sistema
	std::string command = "python3 " + py + " " + dirPath + " " + root_location + " > ./conf_web/.tmp";
	int returnCode = std::system(command.c_str());

	// Verificar el éxito del comando del sistema
	if (returnCode != 0)
		throw std::system_error(returnCode, std::generic_category(), "Error al ejecutar el comando del sistema");

	// Leer el archivo temporal
	std::ifstream tempFile("./conf_web/.tmp");
	if (!tempFile.good())
		throw std::runtime_error("Error: No se pudo abrir el archivo temporal para leer");

	std::stringstream ss;
	ss << tempFile.rdbuf();
	result = ss.str();

	// Limpiar el archivo temporal
	// if (std::remove("./conf_web/.tmp") != 0)
	// 	std::cerr << "Advertencia: No se pudo eliminar el archivo temporal" << std::endl;

	return result;
}
