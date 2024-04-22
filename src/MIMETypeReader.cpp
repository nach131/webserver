/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MIMETypeReader.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 20:08:48 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/22 21:31:10 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MIMETypeReader.hpp"

MIMETypeReader::MIMETypeReader(const std::string &filePath)
{
	loadMIMETypes(filePath);
}

std::string MIMETypeReader::getContentType(const std::string &extension) const
{
	std::string ext = extension;
	std::transform(ext.begin(), ext.end(), ext.begin(), tolower); // Convertir la extensión a minúsculas

	std::map<std::string, std::string>::const_iterator it = _mimeTypes.find(ext);
	if (it != _mimeTypes.end())
	{
		std::string mime = it->second;
		if (mime.substr(0, 4) == "text")
		{
			// Si el tipo MIME comienza con "text", añade "; charset=UTF-8"
			mime += "; charset=UTF-8";
		}
		return mime;
	}
	else
		return "application/octet-stream"; // Tipo MIME por defecto si no se encuentra la extensión
}

void MIMETypeReader::print()
{
	for (std::map<std::string, std::string>::iterator it = _mimeTypes.begin(); it != _mimeTypes.end(); it++)
		std::cout << it->first << " " << it->second << std::endl;
}

void MIMETypeReader::loadMIMETypes(const std::string &filePath)
{
	std::ifstream file(filePath.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error al abrir el archivo " << filePath << std::endl;
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		if (line.find("types") != std::string::npos)
		{
			// Empezar a leer los tipos MIME después de la línea "types {"
			while (std::getline(file, line) && line.find("}") == std::string::npos)
			{
				if (line.empty())
					continue; // Ignorar líneas vacías
				std::istringstream iss(line);
				std::string type;
				std::vector<std::string> extensions;
				iss >> type;
				std::string extension;
				while (iss >> extension)
					extensions.push_back(extension);
				for (std::vector<std::string>::iterator ext_it = extensions.begin(); ext_it != extensions.end(); ++ext_it)
					_mimeTypes[*ext_it] = type;
			}
			break; // Salir del bucle al finalizar los tipos MIME
		}
	}
	file.close();
}

MIMETypeReader::~MIMETypeReader() {}
