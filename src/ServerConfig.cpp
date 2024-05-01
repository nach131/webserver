/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 15:05:34 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/01 00:30:39 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig(const std::string &mimeFilePath) : _port(8080), _apiPort(3000), _apiForward("192.168.1.20"), _mime(mimeFilePath), _first(true) {} // Valores por defecto

ServerConfig::~ServerConfig() {}

void ServerConfig::loadConf(const std::string &filename)
{
	(void)filename;
	_serverName = "locahost";
	_rootDirectory = "dist/";

	_errorPages[404] = "config_web/error/404.html";
	_errorPages[405] = "config_web/error/405.html";

	_locations["/"] = std::map<std::string, std::string>();
	_locations["/"]["autoindex"] = "off";
	_locations["/"]["allow_methods"] = "DELETE POST GET";
	_locations["/files"] = std::map<std::string, std::string>();
	_locations["/files"]["autoindex"] = "on";
	_locations["/files"]["allow_methods"] = "GET";
	_locations["/cgi_bin"] = std::map<std::string, std::string>();
	_locations["/cgi_bin"]["autoindex"] = "off";
	_locations["/cgi_bin"]["allow_methods"] = "GET";
	_locations["/cgi_bin"]["root"] = "./";
	_locations["/cgi_bin"]["index"] = "calc.py";
	_locations["/cgi_bin"]["cgi_path"] = "/usr/bin/python3 /bin/bash";
	_locations["/cgi_bin"]["cgi_ext"] = "cgi_ext .py .sh";
}

void ServerConfig::print() const
{
	std::cout << BLUE;
	std::cout << "[ Server Configuration ]" << std::endl;
	std::cout << "Port: " << _port << std::endl;
	std::cout << "Server Name: " << _serverName << std::endl;
	std::cout << "Root Directory: " << _rootDirectory << std::endl;
	std::cout << "Error Pages:" << std::endl;
	for (std::map<int, std::string>::const_iterator it = _errorPages.begin(); it != _errorPages.end(); ++it)
		std::cout << "  " << it->first << ": " << it->second << std::endl;
	std::cout << "API Forward: " << _apiForward << std::endl;
	std::cout << "API Port: " << _apiPort << std::endl;
	std::cout << "first GET: " << _first << std::endl;

	std::cout << "Locations:" << std::endl;
	for (std::map<std::string, std::map<std::string, std::string> >::const_iterator it = _locations.begin(); it != _locations.end(); ++it)
	{
		std::cout << "  " << it->first << ":" << std::endl;
		for (std::map<std::string, std::string>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
			std::cout << "    " << it2->first << ": " << it2->second << std::endl;
	}
	std::cout << RESET;
}

int ServerConfig::getPort() const { return _port; }

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

std::string ServerConfig::getApiForward() const { return _apiForward; }

int ServerConfig::getApiPort() const { return _apiPort; }

std::map<std::string, std::map<std::string, std::string> > ServerConfig::getLocation() const
{
	return _locations;
}

// Método para obtener el tipo MIME de una extensión de archivo
std::string ServerConfig::getContentType(const std::string &extension) const
{
	return _mime.getContentType(extension);
}
