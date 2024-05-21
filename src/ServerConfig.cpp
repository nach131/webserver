/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 15:05:34 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/21 12:48:42 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig(const std::vector<std::string> &content, const std::string &mimeFilePath) : _port(8080), _apiPort(3000), _apiForward("192.168.1.20"), _mime(mimeFilePath) // Valores por defecto
{
	fillVariables(content);

	memset(&_serverAddress, 0, sizeof(_serverAddress));
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_port = htons(_port);				// Puerto del servidor
	_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // Escuchar en todas las interfaces de red

	// Crear socket
	_serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocketFd < 0)
	{
		std::string errorMsg = RED "Error creating socket:\n";
		errorMsg += CYAN;
		errorMsg += strerror(errno);
		throw std::runtime_error(errorMsg);
	}

	// Enlazar el socket a la dirección del servidor
	if (bind(_serverSocketFd, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress)) < 0)
	{
		std::string errorMsg = RED "Socket binding error:\n";
		errorMsg += CYAN + std::to_string(ntohs(_serverAddress.sin_port)) + " ";
		errorMsg += strerror(errno);
		throw std::runtime_error(errorMsg);
	}

	// Escuchar por conexiones entrantes
	listen(_serverSocketFd, 5);

	_buffer = new char[1024];
	memset(_buffer, 0, 1024);
}

ServerConfig::~ServerConfig() { delete _buffer; }

void ServerConfig::print() const
{
	std::cout << BLUE;
	std::cout << "[ Server Configuration ]" << std::endl;
	std::cout << "Port: " << _port << std::endl;
	std::cout << "Server Name: " << _serverName << std::endl;
	// std::cout << "Root Directory: " << _rootDirectory << std::endl;
	std::cout << "Error Pages:" << std::endl;
	for (std::map<int, std::string>::const_iterator it = _errorPages.begin(); it != _errorPages.end(); ++it)
		std::cout << "  " << it->first << ": " << it->second << std::endl;
	std::cout << "API Forward: " << _apiForward << std::endl;
	std::cout << "API Port: " << _apiPort << std::endl;

	std::cout << "Locations:" << std::endl;
	for (std::map<std::string, std::map<std::string, std::string> >::const_iterator it = _locations.begin(); it != _locations.end(); ++it)
	{
		std::cout << "  " << it->first << ":" << std::endl;
		for (std::map<std::string, std::string>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
			std::cout << "    " << it2->first << ": " << it2->second << std::endl;
	}
	std::cout << RESET;
}

void ServerConfig::printLocations()
{
	std::cout << "Locations:" << std::endl;
	for (std::map<std::string, std::map<std::string, std::string> >::const_iterator it = _locations.begin(); it != _locations.end(); ++it)
	{
		std::cout << "  " << it->first << ":" << std::endl;
		for (std::map<std::string, std::string>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
			std::cout << "    " << it2->first << ": " << it2->second << std::endl;
	}
}

// int ServerConfig::getApiPort() const { return _apiPort; }

// std::string ServerConfig::getApiForward() const { return _apiForward; }

int ServerConfig::getPort() const { return _port; }

int ServerConfig::getServerSocket() const { return _serverSocketFd; }

std::string ServerConfig::getServerName() const { return _serverName; }

std::string ServerConfig::getRootDirectory() const { return _rootDirectory; }

std::string ServerConfig::getErrorPage(int errorCode) const
{
	std::map<int, std::string>::const_iterator it = _errorPages.find(errorCode);
	if (it != _errorPages.end())
		return it->second;
	else
		return ""; // Opción por defecto si no se encuentra la página de error
}

// Método para obtener el tipo MIME de una extensión de archivo
std::string ServerConfig::getContentType(const std::string &extension) const
{
	return _mime.getContentType(extension);
}

// std::map<std::string, std::map<std::string, std::string> > ServerConfig::getLocation() const
// {
// 	return _locations;
// }

char *ServerConfig::getBuffer() const { return _buffer; }

void ServerConfig::setBuffer(char *buf) { _buffer = buf; }

// Obtiene la configuración de la ubicación correspondiente al path
LocationResult ServerConfig::getLocationConfig(const std::string &path)
{
	std::string best_match = "/";
	std::map<std::string, std::map<std::string, std::string> >::iterator it;
	for (it = _locations.begin(); it != _locations.end(); ++it)
	{
		if (path.find(it->first) == 0 && it->first.length() > best_match.length())
		{
			best_match = it->first;
		}
	}

	LocationResult result;
	result.name = best_match;
	result.config = _locations[best_match];
	return result;
}

// std::string ServerConfig::getRoot(const std::string &location) const
// {
// 	std::map<std::string, std::map<std::string, std::string> >::const_iterator locIt = _locations.find(location);
// 	if (locIt != _locations.end())
// 	{
// 		std::map<std::string, std::string>::const_iterator rootIt = locIt->second.find("root");
// 		if (rootIt != locIt->second.end())
// 		{
// 			return rootIt->second;
// 		}
// 	}
// 	return "";
// }

// std::string ServerConfig::getIndex(const std::string &location) const
// {
// 	std::map<std::string, std::map<std::string, std::string> >::const_iterator locIt = _locations.find(location);
// 	if (locIt != _locations.end())
// 	{
// 		std::map<std::string, std::string>::const_iterator rootIt = locIt->second.find("index");
// 		if (rootIt != locIt->second.end())
// 		{
// 			return rootIt->second;
// 		}
// 	}
// 	return "";
// }

// bool ServerConfig::isLocationAllowed(const std::string &path)
// {
// 	std::map<std::string, std::map<std::string, std::string> >::const_iterator it = _locations.find(path);
// 	if (it != _locations.end())
// 		return true;
// 	return false;
// }

// struct sockaddr_in ServerConfig::getServerAddress() const { return _serverAddress; }

int ServerConfig::checkLine(const std::string &line)
{
	size_t l = 0;
	while (line[l] == ' ' || line[l] == '\t')
		l++;
	if (line[l] != '#' && line[l] != '\0')
		return 1;
	return 0;
}

void ServerConfig::addValue(std::stringstream &ss)
{
	std::string key, err, value;
	ss >> key;
	std::vector<std::string>::iterator it = std::find(_kv.getKeys().begin(), _kv.getKeys().end(), key);
	switch (it - _kv.getKeys().begin())
	{
	case 0:
	case 2:
	case 3:
	case 6:
	case 8:
	case 9:
	case 10:
		ss >> value;
		takeOutSemiColumn(value);
		break;
	case 5:
		ss >> err;
		ss >> value;
		takeOutSemiColumn(value);
		break;
	}
	switch (it - _kv.getKeys().begin())
	{
	case 0:
		_rootDirectory = value;
		break;
	case 1:
		// TODO index: Ver lo que hay que hacer aqui
		break;
	case 2:
		_port = std::atoi(value.c_str());
		break;
	case 3:
		_apiPort = std::atoi(value.c_str());
		break;
	case 4:
		// TODO autoindex: Ver lo que hay que hacer aqui
		break;
	case 5:
		_errorPages[std::atoi(err.c_str())] = value;
		break;
	case 6:
		_serverName = value;
		break;
	case 7:
		// TODO allow_methods: Ver lo que hay que hacer aqui
		break;
	default:
		// TODO client_max_body_size: Ver lo que hay que hacer aqui
		break;
	}
}

void ServerConfig::fillVariables(const std::vector<std::string> &arr)
{
	std::string el;
	for (size_t i = 0; i < arr.size(); i++)
	{
		if (arr[i].find("location") != std::string::npos)
		{
			std::string path;
			std::stringstream ss(arr[i]);
			ss >> path >> path;
			for (i++; arr[i].find("}") == std::string::npos && checkLine(arr[i]) && i < arr.size(); i++)
			{
				std::string el, key, value;
				std::stringstream ss(arr[i]);
				ss >> key;
				while (ss >> el)
				{
					if (value[0] == '\0')
						value.append(el);
					else
						value.append(" " + el);
				}
				takeOutSemiColumn(value);
				_locations[path] = std::map<std::string, std::string>();
				_locations[path][key] = value;
			}
		}
		else if (arr[i].length() == 0 || !checkLine(arr[i]) ||
				 arr[i].find("{") != std::string::npos ||
				 arr[i].find("}") != std::string::npos)
			continue;
		else
		{
			std::stringstream ss(arr[i]);
			addValue(ss);
		}
	}
}
