/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PythonExec.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 10:02:31 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/31 13:03:34 by nmota-bu         ###   ########.fr       */
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
	std::string command = "python3 " + py + " > ./.tmpdir/.tmp";
	int returnCode = std::system(command.c_str());

	// Verificar el éxito del comando del sistema
	if (returnCode != 0)
		throw std::system_error(returnCode, std::generic_category(), "Error al ejecutar el comando del sistema");

	// Leer el archivo temporal
	std::ifstream tempFile("./.tmpdir/.tmp");
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
	std::string dataTmp = data;

	std::cout << YELLOW;

	std::cout << "execPython DOS\n ";
	std::cout << " py: " << py << std::endl;
	std::cout << " data: " << data << RESET << std::endl;

	removeLastNewline(dataTmp);
	// Construir el comando del sistema
	std::string command = "python3 " + py + " \"" + dataTmp + "\" > ./.tmpdir/.tmp";
	int returnCode = std::system(command.c_str());

	// Verificar el éxito del comando del sistema
	if (returnCode != 0)
		throw std::system_error(returnCode, std::generic_category(), "Error al ejecutar el comando del sistema");

	// Leer el archivo temporal
	std::ifstream tempFile("./.tmpdir/.tmp");
	if (!tempFile.good())
		throw std::runtime_error("Error: No se pudo abrir el archivo temporal para leer");

	std::stringstream ss;
	ss << tempFile.rdbuf();
	result = ss.str();

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
	std::string command = "python3 " + py + " " + dirPath + " " + root_location + " > ./.tmpdir/.tmp";
	int returnCode = std::system(command.c_str());

	// Verificar el éxito del comando del sistema
	if (returnCode != 0)
		throw std::system_error(returnCode, std::generic_category(), "Error al ejecutar el comando del sistema");

	// Leer el archivo temporal
	std::ifstream tempFile("./.tmpdir/.tmp");
	if (!tempFile.good())
		throw std::runtime_error("Error: No se pudo abrir el archivo temporal para leer");

	std::stringstream ss;
	ss << tempFile.rdbuf();
	result = ss.str();

	// Limpiar el archivo temporal
	// if (std::remove("./.tmpdir/.tmp") != 0)
	// 	std::cerr << "Advertencia: No se pudo eliminar el archivo temporal" << std::endl;

	return result;
}

std::string execPythonFile(const std::string &py, const std::string &filename, const std::string &data)
{
	std::string result;

	std::cout << YELLOW;
	std::cout << "execPythonFile TRES\n";
	std::cout << " py: " << py << std::endl;
	std::cout << " filename: " << filename << std::endl;
	std::cout << " data: " << data << std::endl;
	std::cout << RESET;

	// Escape quotes and special characters in data
	// std::string escaped_data = "";
	// for (size_t i = 0; i < data.size(); ++i)
	// {
	// 	if (data[i] == '\"' || data[i] == '\\')
	// 		escaped_data += '\\';
	// 	escaped_data += data[i];
	// }

	// Form the command string
	std::string command = "python3 " + py + " \"" + filename + "\" " + data + " > ./tmp.txt";
	// std::string command = "python3 " + py + " \"" + filename + "\" \"" + escaped_data + "\" > ./tmp.txt";

	int returnCode = std::system(command.c_str());

	// Verificar el éxito del comando del sistema
	if (returnCode != 0)
		throw std::system_error(returnCode, std::generic_category(), "Error al ejecutar el comando del sistema");

	// Leer el archivo temporal
	std::ifstream tempFile("./.tmpdir/.tmp");
	if (!tempFile.good())
		throw std::runtime_error("Error: No se pudo abrir el archivo temporal para leer");

	std::stringstream ss;
	ss << tempFile.rdbuf();
	result = ss.str();

	return result;
}
