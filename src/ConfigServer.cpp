/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 20:29:26 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/11 18:55:52 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/tmp/ConfigServer.hpp"

/*
ConfigServer::ConfigServer(const std::vector<std::string> & arr, size_t & i)
{
	int brack = 1;

	for (; i < arr.size() && brack != 0; i++)
	{
		std::string key, val;
		std::stringstream ss(arr[i]);
		ss >> key;
		// TODO: Controlar que la key este' bien
		while (ss >> val)
		{
			// TODO: Controlar que los valores esten bien
		}
	}
}
*/

ConfigServer::ConfigServer(const std::string &mimeFilePath) : _first(true), _port(8080), _apiPort(3000), _apiForward("192.168.1.20"), _mime(mimeFilePath) {} // Valores por defecto

ConfigServer::~ConfigServer() {}

void ConfigServer::loadConf(const std::string &filename)
{
//	_serverName = "locahost";
//	_rootDirectory = "dist/";
//
//	_errorPages[404] = "config_web/error/404/index.html";
//	_errorPages[405] = "config_web/error/405/index.html";
//
//	_locations["/"] = std::map<std::string, std::string>();
//	_locations["/"]["autoindex"] = "off";
//	_locations["/"]["allow_methods"] = "DELETE POST GET";
//	_locations["/files"] = std::map<std::string, std::string>();
//	_locations["/files"]["autoindex"] = "on";
//	_locations["/files"]["allow_methods"] = "GET";
//	_locations["/cgi_bin"] = std::map<std::string, std::string>();
//	_locations["/cgi_bin"]["autoindex"] = "off";
//	_locations["/cgi_bin"]["allow_methods"] = "GET";
//	_locations["/cgi_bin"]["root"] = "./";
//	_locations["/cgi_bin"]["index"] = "calc.py";
//	_locations["/cgi_bin"]["cgi_path"] = "/usr/bin/python3 /bin/bash";
//	_locations["/cgi_bin"]["cgi_ext"] = "cgi_ext .py .sh";

	std::string line;
	std::ifstream in(filename);
	std::vector<std::string> arr;

	while (getline(in, line))
	{
		std::cout << "Line: -" << (int)(line[0] - 48) << "-" << std::endl;
		arr.push_back(line);
	}
	in.close();
}

void ConfigServer::print() const
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

int ConfigServer::getPort() const { return _port; }

std::string ConfigServer::getServerName() const { return _serverName; }

std::string ConfigServer::getRootDirectory() const { return _rootDirectory; }

std::string ConfigServer::getErrorPage(int errorCode) const
{
	std::map<int, std::string>::const_iterator it = _errorPages.find(errorCode);
	if (it != _errorPages.end())
		return it->second;
	else
		return ""; // Opción por defecto si no se encuentra la página de error
}

std::string ConfigServer::getApiForward() const { return _apiForward; }

int ConfigServer::getApiPort() const { return _apiPort; }

std::map<std::string, std::map<std::string, std::string> > ConfigServer::getLocation() const
{
	return _locations;
}

// Método para obtener el tipo MIME de una extensión de archivo
std::string ConfigServer::getContentType(const std::string &extension) const
{
	return _mime.getContentType(extension);
}

bool ConfigServer::getFirst() const { return _first;}

std::string ConfigServer::getPrePath() const {return _prePath;}

void ConfigServer::setFirst(bool action){	_first = action;}

void ConfigServer::setPrePath(const std::string &path) { _prePath = path; }

ConfigServer::ConfErrorException::ConfErrorException (const std::string & msg)
	: std::runtime_error("\033[0;31mError: " + msg) {}
