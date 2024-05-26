/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 17:46:33 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/26 14:28:35 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() {}

Location::~Location() {}

void Location::init(const LocationResult &location, const std::string &path, const std::string &referer)
{

	_location = location;
	bool autoindex = autoIndexOn();

	std::string alias = getAlias();
	std::string root = getRoot();
	// std::string pathRoot = root.empty() ? alias : root;


	std::string index = getIndex().empty() ? "/index.html" : getIndex();
	std::string fileName = isFile(path) && !isFileLocation() ? getFileName(path) : index;
	std::string pathFile = removeSubstring(path, location.name);



	// std::string referer_location = removeBeforeNumber(referer, "8080");
	std::string referer_location = _location.name;

	std::cout << MAGENTA;
	std::cout << " path: " << path << std::endl;
	std::cout << " referer: " << referer << std::endl;
	std::cout << " referer_location: " << referer_location << std::endl;

	std::cout << "==============\n";
	// std::cout << "refer + path: " << referer_location + path << std::endl;
	std::cout << "==============\n";

	std::cout << " isFileLocation : " << isFileLocation() << std::endl;
	std::cout << " index: " << index << std::endl;
	std::cout << " fileName: " << fileName << std::endl;
	
	// std::cout << "pathRoot: " << pathRoot<< std::endl;
	std::cout << " alias: " << alias << std::endl;
	std::cout << " root: " << root << std::endl;
	std::cout << "quita Location: " << removeSubstring(path, location.name) << std::endl;
	std::cout << RESET;

	// if(!root.empty())

	if (autoindex)
	{
		std::cout << "ON CARPETA\n";
		if (!alias.empty())
		{
			std::cout << "!alias.empty if\n";
			// if (!referer_location.empty())
			if (_location.name == path)
			{
				std::cout << " _location.name == path: " << alias << std::endl;
				_location.realPath = alias;
			}
			else if (!alias.empty() && !referer.empty())
			{
				std::cout << " !alias.empty !referer.empty: " << alias + path << std::endl;
				_location.realPath = alias + path;
			}
			else if (!_location.name.empty()) // antes estaba !referr
			{
				std::cout << " !alias.empty !_location.name.empty() if: " << alias + "/" + fileName << std::endl;
				_location.realPath = alias + "/" + fileName;
				// solucionar el path en python segunda carpeta los ficheros no cargan
			}
			else
			{
				std::cout << " !alias.empty !_location.name.empty() else: " << alias << std::endl;
				_location.realPath = alias;
			}
		}
		else
		{
			// TODAS LAS CARPETAS Y Los Archivo en raiz
			std::cout << "!alias.empty else: " << root + path << std::endl;
			_location.realPath = root + path;
		}
	}
	else
	{
		if (isFile(pathFile))
		{
			std::cout << "SI File: " << std::endl;
			if (root.empty())
			{
				std::cout << " alias: " << alias + pathFile << std::endl;
				_location.realPath = alias + pathFile;
			}
			else if (path == _location.name && isFile(_location.name))
			{
				std::cout << " path == _location.name && isFile(_location.name): " << "---" << std::endl;
				_location.realPath = "";
			}
			else
			{
				if (referer.empty() || isFile(_location.name))
				{
					std::cout << "  SI File root sin ref: " << root + path << std::endl;
					_location.realPath = root + path;
				}
				else
				{
					// cuando es location es fichero y hay refere
					// ANTES
					// std::cout << "  root con ref: " << root + referer_location + path << std::endl;
					// _location.realPath = root + referer_location + path;
					// AHORA POR QUE PONE DOS VECES LOCATION
					std::cout << "  root con ref: " << root + path << std::endl;
					_location.realPath = root + path;
				}
			}
		}
		else
		{
			std::cout << "NO File: " << std::endl;
			if (root.empty())
			{
				std::cout << " NO File alias: " << alias + fileName << std::endl;
				_location.realPath = alias + fileName;
			}
			else
			{
				std::cout << " NO File ROOT: " << alias + fileName << std::endl;
				if (path == _location.name && !alias.empty())
				{
					std::cout << "  path == _location.name: " << root << std::endl;
					_location.realPath = "";
				}
				else if (path == _location.name && !root.empty())
				{
				std::cout << "  path == _location.name && !root.empty() " << std::endl;
					if (isFile(_location.name))
					{
						std::cout << "   isFile(_location.name) if: " << root + index << std::endl;
						_location.realPath = root + index;
					}
					else
					// esto nombre location igual peticion
					{
						std::cout << "   isFile(_location.name) else " << root + index << std::endl;
						_location.realPath = root + path + index;
					}
				}
				else if (referer.empty())
				{
					std::cout << "  referer.empty(): " << root + path + fileName << std::endl;
					_location.realPath = root + path + fileName;
				}
				else
				{
					std::cout << "  else: " << std::endl;
					if (path != referer_location)
					{
						std::cout << "   root con ref != path: " << root + referer_location + path + fileName << std::endl;
						_location.realPath = root + referer_location + path + fileName;
					}

					else
					{
						std::cout << "   root con ref == path: " << root + path + fileName << std::endl;
						_location.realPath = root + path + fileName;
					}
				}
			}
		}
	}
}

void Location::init_no(const LocationResult &location, const std::string &path, const std::string &referer)
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

	if (_location.name == path && !index.empty() && !autoindex)
	{
		std::cout << "one\n";
		// if (path == "/")
		// {
		// std::cout << "one if\n";
		// 	_location.realPath = root + path + "/" + index;
		// }
		if (root == "./")
		{
			std::cout << "one  if 1\n";
			_location.realPath = "." + path + "/" + index;
		}
		else
		{
			std::cout << "else \n";
			_location.realPath = root + path + "/" + index;
		}
	}
	else if (_location.name == path && index.empty() && !autoindex)
	{
		std::cout << "dos\n";
		if (path == "/")
		{
			std::cout << "dos if\n";
			_location.realPath = root + path + "/" + "index.html";
		}
		else if (!alias.empty())
		{
			std::cout << "dos else if 1\n";
			_location.realPath = alias + "/index.html";
		}
		else if (!alias.empty())
		{
			std::cout << "dos else  if 2\n";
			_location.realPath = root + "/index.html";
		}
	}
	else if (path == "/" && index.empty() && !autoindex)
	{
		std::cout << "tres\n";
		_location.realPath = root + path + "/" + "index.html";
	}
	else if (!alias.empty())
	{
		std::cout << "cuatro\n";
		_location.realPath = alias;
	}
	else if (_location.name == pathMod && !root.empty())
	{
		std::cout << "cinco\n";
		_location.realPath = root + pathMod;
	}
	else if (!root.empty() && !referer.empty())
	{
		// _location.realPath = root + path;
		std::cout << "seis\n";
		if (_location.name == "/")
		{

			std::cout << "seis if\n";
			_location.realPath = root + referer = pathMod;
		}
		else if (isFile(pathMod))
		{
			std::cout << "seis else  if\n";
			_location.realPath = root + path;
		}
		else
		{
			std::cout << "seis else\n";
			// _location.realPath = root + referer + path;
			_location.realPath = root + path;
		}
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

bool Location::isFileLocation() { return isFile(_location.name); }

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
	{
		static std::string result;
		result = "/" + it->second;
		return result;
	}

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

//=========================================================================

LocationResult compareLocationResults(const LocationResult &loc1, const LocationResult &loc2)
{
	// Si ambos nombres son iguales a "/", devuelve loc1
	if (loc1.name == "/" && loc2.name == "/")
		return loc1;

	// Si loc1.name es diferente de "/", devuelve loc1
	if (loc1.name != "/")
		return loc1;

	// Si loc1.name es igual a "/" y loc2.name es diferente de "/", devuelve loc2
	if (loc2.name != "/")
		return loc2;

	// Si no se cumple ninguna de las condiciones anteriores, devuelve loc1
	return loc1;
}
