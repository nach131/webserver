/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 15:05:34 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/13 20:15:52 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ServerConfig.hpp"

ServerConfig::ServerConfig(const std::string &mimeFilePath) : _first(true), _port(8080), _apiPort(3000), _apiForward("192.168.1.20"), _mime(mimeFilePath) // Valores por defecto
{
	memset(&_serverAddress, 0, sizeof(_serverAddress));
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_port = htons(8080);				// Puerto del servidor
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

void ServerConfig::loadConf(const std::string &filename)
{
	// (void)filename;
	// _serverName = "locahost";
	// _rootDirectory = "dist/";

	// _errorPages[404] = "config_web/error/404/index.html";
	// _errorPages[405] = "config_web/error/405/index.html";

	// _locations["/"] = std::map<std::string, std::string>();
	// _locations["/"]["autoindex"] = "off";
	// _locations["/"]["allow_methods"] = "DELETE POST GET";
	// _locations["/files"] = std::map<std::string, std::string>();
	// _locations["/files"]["autoindex"] = "on";
	// _locations["/files"]["allow_methods"] = "GET";
	// _locations["/cgi_bin"] = std::map<std::string, std::string>();
	// _locations["/cgi_bin"]["autoindex"] = "off";
	// _locations["/cgi_bin"]["allow_methods"] = "GET";
	// _locations["/cgi_bin"]["root"] = "./";
	// _locations["/cgi_bin"]["index"] = "calc.py";
	// _locations["/cgi_bin"]["cgi_path"] = "/usr/bin/python3 /bin/bash";
	// _locations["/cgi_bin"]["cgi_ext"] = "cgi_ext .py .sh";

	std::string line;
	std::ifstream in(filename);
	std::vector<std::string> arr;

	if (!in.good())
		throw e_cee("File doesn't exist");

	while (getline(in, line))
	{
		std::cout << "Line: -" << (int)(line[0] - 48) << "- -" << line[0] << "- -" << line[1] << "-" << std::endl;
		arr.push_back(line);
	}
	in.close();
	try
	{
		checkSyntax(arr);
		fillVariables(arr);
	}
	catch (std::runtime_error &ex)
	{
		throw e_cee(ex.what());
	}
	std::cout << "Syntax correct!" << std::endl;
	exit(0);
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

int ServerConfig::getApiPort() const { return _apiPort; }

int ServerConfig::getServerSocket() const { return _serverSocketFd; }

std::string ServerConfig::getApiForward() const { return _apiForward; }

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

std::map<std::string, std::map<std::string, std::string> > ServerConfig::getLocation() const
{
	return _locations;
}

void ServerConfig::setBuffer(char *buf) { _buffer = buf; }

char *ServerConfig::getBuffer() const { return _buffer; }

// struct sockaddr_in ServerConfig::getServerAddress() const { return _serverAddress; }

bool ServerConfig::getFirst() const { return _first;}

std::string ServerConfig::getPrePath() const {return _prePath;}

void ServerConfig::setFirst(bool action){	_first = action;}

void ServerConfig::setPrePath(const std::string &path) { _prePath = path; }

int ServerConfig::checkLine(std::string & line)
{
	if (line[0] != 9 && line[0] != 0)
		return 0;
	for (size_t i = 1; line[i] != 9 && line[i] != 0; i++)
		if (line[i] == '#')
			return 1;
	return 0;
}

void ServerConfig::checkSyntax(std::vector<std::string> & arr)
{
	bool check = true;
	int brack = 1, lineNum = 1;
	std::stringstream ss;

	for (std::vector<std::string>::iterator it = arr.begin(); it != arr.end(); it++)
	{
		std::cout << "Array line: " << *it << std::endl;
	}
	if (arr[0].find("server {") == std::string::npos)
		parseError("server declaration incorrect at line ", lineNum);
	for (size_t i = 1; i < arr.size(); i++)
	{
		lineNum++;
		check = true;
//		std::cout << "Num: " << lineNum << " Line ->" << arr[i] << std::endl;
		size_t j = 0;
		for ( ; arr[i][j] != 0; j++)
		{
//			std::cout << "Char: -" << arr[i][j] << "- Val:" << (int)arr[i][j] << std::endl;
			if (arr[i][j] == '#')
				check = false;
		}
//		std::cout << "Here Char: -" << arr[i][j] << "- Val:" << (int)arr[i][j] << std::endl;
		if (!check || arr[i].length() == 0 || arr[i][j - 1] == 9)
		{
//			std::cout << "continue" << std::endl;
			continue;
		}
		if (arr[i].find(";") == std::string::npos
			&& arr[i].find("location ") == std::string::npos
			&& arr[i].find("}") == std::string::npos)
			parseError("expected semicolon at line ", lineNum);
		if (arr[i].find("location ") != std::string::npos)
		{
			size_t j = arr[i].find("location ") + 9;
			for ( ; arr[i][j] != '{'; j++)
				if (arr[i][j] != '{')
					break;
			if (arr[i].find("{") == j)
				parseError("expected location path at line ", lineNum);
		}
		for (size_t l = 0; l < arr[i].length(); l++)
		{
			if (arr[i][l] == '{') { brack++; }
			if (arr[i][l] == '}') { brack--; }
			if (brack < 0)
				parseError("expected block before } at line ", lineNum);
			if (arr[i][l] == ';') { break; }
		}
	}
	if (brack > 0)
		parseError("server block not closed at line ", lineNum);
}

void ServerConfig::parseError(std::string str, int n)
{
	std::stringstream ss;
	ss << str << n;
	throw std::runtime_error(ss.str());
}

void ServerConfig::fillVariables(std::vector<std::string> &arr)
{
	std::string line, el;
	for (size_t i = 0; i < arr.size(); i++)
	{
	}
}

ServerConfig::ConfErrorException::ConfErrorException (const std::string & msg)
	: std::runtime_error("\033[0;31mError: " + msg) {}
