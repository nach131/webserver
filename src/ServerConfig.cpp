/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 15:05:34 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/08 19:40:48 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ServerConfig.hpp"

ServerConfig::ServerConfig(const std::string &mimeFilePath) :
	_first(true), _port(8080), _apiPort(3000), _apiForward("192.168.1.20"), _mime(mimeFilePath) {} // Valores por defecto

ServerConfig::~ServerConfig() {}

void ServerConfig::loadConf(const std::string &filename)
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
		if (line[0] == '\0' || checkLine(line))
			continue;
		arr.push_back(line);
	}
	in.close();
	try { checkSyntax(arr); }
	catch (std::runtime_error & ex) { throw e_cee(ex.what()); }
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
	std::cout << "pre Path: " << _prePath << std::endl;

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

bool ServerConfig::getFirst() const { return _first;}

std::string ServerConfig::getPrePath() const {return _prePath;}

void ServerConfig::setFirst(bool action){	_first = action;}

void ServerConfig::setPrePath(const std::string &path) { _prePath = path; }

int ServerConfig::checkLine(std::string & line)
{
	for (std::string::iterator it = line.begin(); it != line.end(); it++)
	{
		if (*it != 9 && *it != 0)
			return 0;
	}
	return 1;
}

void ServerConfig::checkSyntax(std::vector<std::string> & arr)
{
	int brack, lineNum = 0;
	lineNum++;

	if (arr[0].find("server {") == std::string::npos)
		throw std::runtime_error("server declaration incorrect at line " + lineNum);
	for (size_t i = 1; i < arr.size(); i++)
	{
		brack++;
		lineNum++;
		if (arr[i].find(";") == std::string::npos
			&& arr[i].find("location {") == std::string::npos)
			throw std::runtime_error("expected semicolon at line " + lineNum);
		for (size_t l = 0; l < arr[i].length(); l++)
		{
			if (arr[i][l] == '{') { brack++; }
			if (arr[i][l] == '}') { brack--; }
			if (brack < 0) { throw std::runtime_error("expected block before } at line " + lineNum); }
			if (arr[i][l] == ';') { break; }
		}
		lineNum++;
	}
	if (brack > 0) { throw std::runtime_error("server block not closed at line " + lineNum); }
}

ServerConfig::ConfErrorException::ConfErrorException (const std::string & msg)
	: std::runtime_error("\033[0;31mError: " + msg) {}
