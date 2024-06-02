/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 15:05:34 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/06/02 13:23:00 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig(const std::vector<std::string> &content, const std::string &mimeFilePath, int len) : _port(8080), _apiPort(3000), _apiForward("192.168.1.20"), _mime(mimeFilePath) // Valores por defecto
{
	fillVariables(content, len);
}

ServerConfig::~ServerConfig() { delete _data; }

// void ServerConfig::print() const
// {
// 	std::cout << BLUE;
// 	std::cout << "[ Server Configuration ]" << std::endl;
// 	std::cout << "Port: " << _port << std::endl;
// 	std::cout << "Server Name: " << _serverName << std::endl;
// 	// std::cout << "Root Directory: " << _rootDirectory << std::endl;
// 	std::cout << "Error Pages:" << std::endl;
// 	for (std::map<int, std::string>::const_iterator it = _errorPages.begin(); it != _errorPages.end(); ++it)
// 		std::cout << "  " << it->first << ": " << it->second << std::endl;
// 	std::cout << "API Forward: " << _apiForward << std::endl;
// 	std::cout << "API Port: " << _apiPort << std::endl;

// 	std::cout << "Locations:" << std::endl;
// 	for (std::map<std::string, std::map<std::string, std::string> >::const_iterator it = _locations.begin(); it != _locations.end(); ++it)
// 	{
// 		std::cout << "  " << it->first << ":" << std::endl;
// 		for (std::map<std::string, std::string>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
// 			std::cout << "    " << it2->first << ": " << it2->second << std::endl;
// 	}
// 	std::cout << RESET;
// }

int ServerConfig::getData() { return _data[0]; }

void ServerConfig::setData(int *data) { _data = data; }

int ServerConfig::getPort() const { return _port; }

int ServerConfig::getServerSocket() const { return _sockFd; }

void ServerConfig::setServerSocket(const int val) { _sockFd = val; }

std::string ServerConfig::getServerName() const { return _serverName; }

std::string ServerConfig::getErrorPage(int errorCode) const
{
	std::map<int, std::string>::const_iterator it = _errorPages.find(errorCode);
	if (it != _errorPages.end())
		return it->second;
	else
		return "";
}

std::string ServerConfig::getContentType(const std::string &extension) const
{
	return _mime.getContentType(extension);
}

LocationResult ServerConfig::getLocationConfig(const std::string &path)
{
	std::string best_match = "/";
	std::map<std::string, std::map<std::string, std::string> >::iterator it;
	for (it = _locations.begin(); it != _locations.end(); ++it)
	{
		if (path.find(it->first) == 0 && it->first.length() > best_match.length())
			best_match = it->first;
	}
	LocationResult result;
	result.name = best_match;
	result.config = _locations[best_match];
	return result;
}

int ServerConfig::checkLine(const std::string &line)
{
	size_t l = 0;
	while (line[l] == ' ' || line[l] == '\t')
		l++;
	if (line[l] != '#' && line[l] != '\0')
		return 1;
	return 0;
}

void ServerConfig::addValue(std::stringstream &ss, int lineNum)
{
	std::string key, err, value;
	ss >> key;
	std::vector<std::string>::iterator it = std::find(_kv.getKeys().begin(), _kv.getKeys().end(), key);
	switch (it - _kv.getKeys().begin())
	{
	case 0:
	case 3:
	case 4:
	case 7:
	case 9:
	case 10:
	case 11:
		ss >> value;
		takeOutSemiColumn(value);
		break;
	case 6:
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
		error("alias not in location at line ", lineNum + 1);
		break;
	case 2:
		error("index not in location at line ", lineNum + 1);
		break;
	case 3:
		_port = std::atoi(value.c_str());
		break;
	case 4:
		_apiPort = std::atoi(value.c_str());
		break;
	case 5:
		error("autoindex not in location at line ", lineNum + 1);
		break;
	case 6:
		_errorPages[std::atoi(err.c_str())] = value;
		break;
	case 7:
		_serverName = value;
		break;
	case 8:
		error("allow_methods not in location at line ", lineNum + 1);
		break;
	default:
		_maxBody = std::atol(value.c_str());
		break;
	}
}

void ServerConfig::fillVariables(const std::vector<std::string> &arr, int len)
{
	std::string el;
	for (size_t i = 0; i < arr.size(); i++)
	{
		if (arr[i].find("location") != std::string::npos)
		{
			std::string path;
			std::stringstream ss(arr[i]);
			ss >> path >> path;
			for (i++; arr[i].find("}") == std::string::npos && i < arr.size(); i++)
			{
				if (!checkLine(arr[i]))
					continue;
				std::string el, key, value;
				std::stringstream ss(arr[i]);
				ss >> key;
				while (ss >> el)
				{
					if (el[0] == ';')
						break;
					if (value[0] == '\0')
						value.append(el);
					else
						value.append(" " + el);
				}
				takeOutSemiColumn(value);
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
			addValue(ss, len + i);
		}
	}
}

void ServerConfig::error(const std::string msg, int lineNum)
{
	std::stringstream ss;
	ss << msg << lineNum;
	throw e_cee(ss.str());
}

ServerConfig::ConfErrorException::ConfErrorException(const std::string &msg)
	: std::runtime_error("\033[0;31mError: " + msg) {}
