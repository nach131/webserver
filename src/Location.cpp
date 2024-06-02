/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 17:46:33 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/06/02 13:42:00 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() {}

Location::~Location() {}

void Location::init(const LocationResult &location, const std::string &path, const std::string &referer, const bool &ref)
{
	_location = location;
	_location.ref = referer;
	bool autoindex = autoIndexOn();
	std::string alias = getAlias();
	std::string root = getRoot();
	std::string index = getIndex().empty() ? "/index.html" : getIndex();
	std::string fileName = isFile(path) && !isFileLocation() ? getFileName(path) : index;
	std::string pathFile = removeSubstring(path, location.name);
	if (autoindex)
	{
		if (!alias.empty())
		{
			if (_location.name == path)
				_location.realPath = alias;
			else if (!alias.empty() && !referer.empty())
				_location.realPath = alias + path;
			else if (!_location.name.empty())
				_location.realPath = alias + "/" + fileName;
			else
				_location.realPath = alias;
		}
		else
			_location.realPath = root + path;
	}
	else
	{
		if (isFile(pathFile))
		{
			if (root.empty())
				_location.realPath = alias + pathFile;
			else if (path == _location.name && isFile(_location.name))
				_location.realPath = "";
			else
			{
				if (referer.empty() || isFile(_location.name))
					_location.realPath = root + path;
				else
				{
					if (!ref)
						_location.realPath = root + path;
					else if (pathStartsWithLocation(path, _location.name))
						_location.realPath = root + path;
					else
						_location.realPath = root + _location.name + path;
				}
			}
		}
		else
		{
			if (root.empty())
				_location.realPath = alias + fileName;
			else
			{
				if (path == _location.name && !alias.empty())
					_location.realPath = "";
				else if (path == _location.name && !root.empty())
				{
					if (isFile(_location.name))
						_location.realPath = root + index;
					else
						_location.realPath = root + path + index;
				}
				else if (referer.empty())
					_location.realPath = root + path + fileName;
				else
				{
					if (path != _location.name)
					{
						std::string toma = !_location.name.find(path) ? _location.name + path : path;
						_location.realPath = root + toma + fileName;
					}
					else
						_location.realPath = root + path + fileName;
				}
			}
		}
	}
	if (!_location.realPath.empty())
		removeDoubleSlashes(_location.realPath);
}

void Location::init_no(const LocationResult &location, const std::string &path, const std::string &referer)
{
	_location = location;
	std::string pathMod = path;
	removeLastSlash(pathMod);
	std::string alias = getAlias();
	std::string root = getRoot();
	std::string index = getIndex();
	bool autoindex = autoIndexOn();
	if (_location.name == path && !index.empty() && !autoindex)
	{
		if (root == "./")
			_location.realPath = "." + path + "/" + index;
		else
			_location.realPath = root + path + "/" + index;
	}
	else if (_location.name == path && index.empty() && !autoindex)
	{
		if (path == "/")
			_location.realPath = root + path + "/" + "index.html";
		else if (!alias.empty())
			_location.realPath = alias + "/index.html";
		else if (!alias.empty())
			_location.realPath = root + "/index.html";
	}
	else if (path == "/" && index.empty() && !autoindex)
		_location.realPath = root + path + "/" + "index.html";
	else if (!alias.empty())
		_location.realPath = alias;
	else if (_location.name == pathMod && !root.empty())
		_location.realPath = root + pathMod;
	else if (!root.empty() && !referer.empty())
	{
		if (_location.name == "/")
			_location.realPath = root + referer = pathMod;
		else if (isFile(pathMod))
			_location.realPath = root + path;
		else
			_location.realPath = root + path;
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
		while (iss >> word)
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

std::string const &Location::getRef() const { return _location.ref; }

LocationResult compareLocationResults(const LocationResult &loc1, const LocationResult &loc2)
{
	if (loc1.name == "/" && loc2.name == "/")
		return loc1;
	if (loc1.name != "/")
		return loc1;
	if (loc2.name != "/")
		return loc2;
	return loc1;
}
