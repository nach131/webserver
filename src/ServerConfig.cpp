/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 15:05:34 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/17 14:15:20 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig(const std::string &mimeFilePath) : _port(8080), _apiPort(3000), _apiForward("192.168.1.20"), _mime(mimeFilePath), _first(true) {} // Valores por defecto

ServerConfig::~ServerConfig() {}

void ServerConfig::loadConf(const std::string &filename)
{
	(void)filename;
	_serverName = "locahost";
	_rootDirectory = "./dist";

	_errorPages[404] = "./config_web/error/404/index.html";
	_errorPages[405] = "./config_web/error/405/index.html";

	// root
	_locations["/"] = std::map<std::string, std::string>();
	_locations["/"]["autoindex"] = "off";
	_locations["/"]["root"] = "./dist";
	_locations["/"]["allow_methods"] = "DELETE POST GET";
	// files
	_locations["/files"] = std::map<std::string, std::string>();
	_locations["/files"]["autoindex"] = "on";
	_locations["/files"]["root"] = "./dist";
	_locations["/files"]["allow_methods"] = "GET";
	_locations["/files"]["index"] = "files.html";
	// other files
	_locations["/other_files"] = std::map<std::string, std::string>();
	_locations["/other_files"]["autoindex"] = "on";
	_locations["/other_files"]["root"] = "./dist";
	_locations["/other_files"]["allow_methods"] = "GET";
	// colors
	_locations["/colors"] = std::map<std::string, std::string>();
	_locations["/colors"]["autoindex"] = "off";
	_locations["/colors"]["root"] = "./dist";
	_locations["/colors"]["allow_methods"] = "GET";
	_locations["/colors"]["index"] = "colors.html";
	// cgi
	_locations["/cgi_bin"] = std::map<std::string, std::string>();
	_locations["/cgi_bin"]["autoindex"] = "off";
	_locations["/cgi_bin"]["root"] = "/Users/nmota-bu/Desktop/webserver";
	_locations["/cgi_bin"]["allow_methods"] = "GET";
	_locations["/cgi_bin"]["index"] = "photo.py";
	// TEST 403 Forbidden
	_locations["/_TEST"] = std::map<std::string, std::string>();
	_locations["/_TEST"]["autoindex"] = "off";
	_locations["/_TEST"]["root"] = "./dist";
	_locations["/_TEST"]["allow_methods"] = "GET";
// EARTH
	_locations["/earth"] = std::map<std::string, std::string>();
	_locations["/earth"]["autoindex"] = "off";
	_locations["/earth"]["root"] = "./dist_1";
	_locations["/earth"]["allow_methods"] = "GET";

	// NO ALLOWED
	_locations["/nada"] = std::map<std::string, std::string>();
	_locations["/nada"]["allow_methods"] = "";

}

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
	std::cout << "first GET: " << _first << std::endl;
	std::cout << "prePath: " << _prePath << std::endl;

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

std::string ServerConfig::getRoot(const std::string &location) const {
    std::map<std::string, std::map<std::string, std::string> >::const_iterator locIt = _locations.find(location);
    if (locIt != _locations.end()) {
        std::map<std::string, std::string>::const_iterator rootIt = locIt->second.find("root");
        if (rootIt != locIt->second.end()) {
            return rootIt->second;
        }
    }
    return "";
}

std::string ServerConfig::getIndex(const std::string &location) const {
    std::map<std::string, std::map<std::string, std::string> >::const_iterator locIt = _locations.find(location);
    if (locIt != _locations.end()) {
        std::map<std::string, std::string>::const_iterator rootIt = locIt->second.find("index");
        if (rootIt != locIt->second.end()) {
            return rootIt->second;
        }
    }
    return "";
}

bool ServerConfig::isLocationAllowed(const std::string &path)
{
	std::map<std::string, std::map<std::string, std::string> >::const_iterator it = _locations.find(path);
	if (it != _locations.end())
		return true;
	return false;
}

bool ServerConfig::isMethodAllowed(const std::string &path, const std::string &method) 
{
	std::map<std::string, std::map<std::string, std::string> >::const_iterator it = _locations.find(path);
	for (std::map<std::string, std::string>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
	{
		std::istringstream iss(it2->second);
		std::string word;
		while (iss >> word) // Leer cada palabra separada por espacios
			if (word == method)
				return true;
	}
	return false;
}

bool ServerConfig::isLocationOn(const std::string &path) 
{
	std::map<std::string, std::map<std::string, std::string> >::const_iterator it = _locations.find(path);
	  if (it != _locations.end()) {
        std::map<std::string, std::string>::const_iterator autoindexIt = it->second.find("autoindex");
        if (autoindexIt != it->second.end()) {
			if(autoindexIt->second == "on")
            return true;
        }
    }
    return false;
}


//=========================================================================

bool ServerConfig::getFirst() const { return _first;}

std::string ServerConfig::getPrePath() const {return _prePath;}

void ServerConfig::setFirst(bool action){	_first = action;}

void ServerConfig::setPrePath(const std::string &path) { _prePath = path; }


// TODO 
// poner 404 en location leerlo y ponerlo en una map con los string
// si no hay 404 poner un generico como el de ejemplo
