/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:05:47 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/27 15:53:49 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"
#include "WebServer.hpp"

HTTPRequest::HTTPRequest() : _last(false) {}

HTTPRequest::HTTPRequest(const HTTPRequest &rhs)
{
	_last = rhs._last;
	_fileName = rhs._fileName;
	_boundary = rhs._boundary;
	_map = rhs._map;
}

HTTPRequest::~HTTPRequest() {}

void HTTPRequest::setMulti(bool &multi) { multi = _last; }

std::string HTTPRequest::getFileName() { return _fileName; }

void HTTPRequest::takeHeader(std::stringstream &ss)
{
	int i = 0;
	std::string first, el, line;
	std::getline(ss, first);
	std::stringstream ssFirst(first);
	while (std::getline(ssFirst, el, ' '))
	{
		if (!el.empty() && el.back() == '\r')
			el.pop_back();
		switch (i)
		{
		case 0:
			_map["Method"] = el;
			break;
		case 1:

			_map["Path"] = el;
			break;
		case 2:
			_map["Version"] = el;
			break;
		}
		i++;
	}
	while (std::getline(ss, line))
	{
		if (line.length() == 1)
			break;
		size_t pos = line.find(':');
		if (pos != std::string::npos)
		{
			std::string name = line.substr(0, pos);
			std::string val = line.substr(pos + 2);
			_map[name] = val;
		}
	}
	for (std::map<std::string, std::string>::iterator it = _map.begin(); it != _map.end(); it++)
	{
		if (it->first.find("Content-Type") != std::string::npos &&
			it->second.find("multipart") != std::string::npos)
		{
			_boundary = it->second.substr(it->second.find("boundary") + 9, it->second.length() - (it->second.find("boundary") + 9) - 1);
			std::cout << std::endl
					  << RED << "Boundary -> -" << _boundary << RESET << std::endl;
			_last = true;
			break;
		}
	}
}

void HTTPRequest::checkBoundary(std::string &content)
{
	std::string line;
	std::stringstream ss(content);
	while (getline(ss, line, '\r'))
	{
		if (line.find(_boundary) != std::string::npos)
		{
			_last = false;
			content.erase(0, line.length());
		}
	}
	_map["Content"] = content;
}

void HTTPRequest::getBuffer(const char *buf, bool &multi)
{
	std::cout << RED << "Buff: ";
	std::cout << buf;
	std::cout << RESET << std::endl;
	if (!multi)
	{
		std::string input(buf);
		std::stringstream ss(input);
		takeHeader(ss);
		std::string line, content;
		while (std::getline(ss, line))
		{
			std::cout << RED << "Line:" << std::endl
					  << line << RESET << std::endl;
			content += line; // Concatenate lines to form the body
		}
		if (multi)
		{
			int count = 0;
			std::stringstream ssContent(content);
			while (std::getline(ssContent, line, '\r'))
			{
				if (line.find(_boundary) == std::string::npos)
					break;
				if (line.find("filename") != std::string::npos)
					_fileName = line.substr(line.find("filename") + 10, line.find_last_of("\"") - (line.find("filename") + 10) - 1);
				content.erase(0, line.length());
				count++;
				if (count == 4)
					break;
			}
		}
		_boundary.append("--");
		checkBoundary(content);
	}
	else
	{
		std::string line, content;
		std::stringstream ss(buf);
		while (std::getline(ss, line))
			content += line; // Concatenate lines to form the body
		checkBoundary(content);
	}
	std::cout << RED << "Content: " << std::endl;
	std::cout << _map["Content"] << std::endl;
}

const std::map<std::string, std::string> &HTTPRequest::getMap() const { return _map; }

const std::string &HTTPRequest::getHeader(const std::string name) const { return _map.find(name)->second; }

// esto esta mal hay que componerlo bien
std::string HTTPRequest::getLocation()
{
	std::string path = getHeader("Path");
	if (path == "/" || path == "/index.html")
		return "/";
	std::vector<std::string> parts = split(path, '/');
	return "/" + parts[0];
}

void HTTPRequest::print()
{
	std::cout << RED << "[ Request client ]" << std::endl;
	for (std::map<std::string, std::string>::iterator it = _map.begin(); it != _map.end(); it++)
	{
		if (it->first == "Content")
			continue;
		std::cout << RED << it->first << ": " << GREEN << it->second << std::endl;
	}
	std::cout << RESET;
}

void HTTPRequest::cleanObject()
{
	_map.clear();
	_boundary.clear();
	_fileName.clear();
}
