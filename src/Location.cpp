/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 17:46:33 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/20 13:12:07 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() {}

Location::~Location() {}

void Location::init(const LocationResult &location, const std::string &path, const std::string &referer)
{
	_location = location;
	std::cout << "LOCATION\n";

std::string pathMod = path;
removeLastSlash(pathMod);

std::cout << "pathMod: " << pathMod << std::endl;

	std::string alias = getAlias();
	std::string root = getRoot();
	std::string index = getIndex();
	bool autoindex = autoIndexOn();

	if (_location.name == path && index != "" && autoindex == false)
	{
		std::cout << "one\n";
		if (path == "/")
			_location.realPath = root + path + "/" + index;
		else if (root == "./")
			_location.realPath = "." + path + "/" + index;
	}
	else if (_location.name == path && index == "" && autoindex == false)
	{
		std::cout << "dos\n";
		if (path == "/")
			_location.realPath = root + path + "/" + "index.html";
	}
	else if (path == "/" && index == "" && autoindex == false)
	{
		std::cout << "tres\n";
		_location.realPath = root + path + "/" + "index.html";
	}
	else if (alias != "")
	{
		std::cout << "cuatro\n";
		_location.realPath = alias;
	}
	else if (_location.name == pathMod && root != "")
	{
		std::cout << "cinco\n";
		_location.realPath = root + pathMod;
	}
	else if (root != "" && referer != "")
	{
		std::cout << "seis\n";
		// std::cout << extractEndpoint(referer) << std::endl;

		// std::cout <<"root: " << root << std::endl;
		// std::cout  << "path: "<< path << std::endl;

		_location.realPath = root + path;
		// _location.realPath = root + "/" + extractEndpoint(referer) + pathMod;

		// std::cout << "realPath" << _location.realPath << std::endl;
	}
	else
			std::cout << "else\n";

}

void Location::print()
{
	std::cout << "LocationName: " << _location.name << std::endl;
	std::cout << "realPath: " << _location.realPath << std::endl;
	std::cout << "=============\n";
	for (std::map<std::string, std::string>::const_iterator it = _location.config.begin(); it != _location.config.end(); ++it)
	{
		std::cout << it->first << " - " << it->second << std::endl;
	}
}

bool Location::autoIndexOn()
{
	std::map<std::string, std::string>::iterator it = _location.config.find("autoindex");
	if (it != _location.config.end())
		return it->second == "on";
	return false;
}

bool Location::methodAllowed(const std::string &method)
{
	std::map<std::string, std::string>::iterator it = _location.config.find("allow_methods");
	if (it != _location.config.end())
	{
		std::istringstream iss(it->second);
		std::string word;
		while (iss >> word) // Leer cada palabra separada por espacios
			if (word == method)
				return true;
	}
	return false;
}

std::string const &Location::getRoot() const
{
	std::map<std::string, std::string>::const_iterator it = _location.config.find("root");
	if (it != _location.config.end())
		return it->second;

	static const std::string empty_string = "";
	return empty_string;
}

std::string const &Location::getIndex() const
{
	std::map<std::string, std::string>::const_iterator it = _location.config.find("index");
	if (it != _location.config.end())
		return it->second;

	static const std::string empty_string = "";
	return empty_string;
}

std::string const &Location::getAlias() const
{
	std::map<std::string, std::string>::const_iterator it = _location.config.find("alias");
	if (it != _location.config.end())
		return it->second;

	static const std::string empty_string = "";
	return empty_string;
}

std::string const &Location::realPath() const { return _location.realPath; }

std::string const &Location::getName() const { return _location.name; }
