/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:05:47 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/27 18:21:40 by vduchi           ###   ########.fr       */
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
	while (getline(ssFirst, el, ' '))
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
	while (getline(ss, line))
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
			std::cout << RED << "Boundary -> -" << _boundary << RESET << std::endl;
			_last = true;
			break;
		}
	}
}

void HTTPRequest::checkBoundary(std::vector<std::string> &content)
{
	for (std::vector<std::string>::iterator it = content.begin(); it != content.end();)
	{
		if ((*it).find(_boundary) != std::string::npos)
		{
			_last = false;
			content.erase(it);
			if (it == content.end())
				break;
			continue;
		}
		it++;
	}
	std::string mapContent;
	for (std::vector<std::string>::iterator it = content.begin(); it != content.end(); it++)
		mapContent += *it;
	_map["Content"] = mapContent;
}

void HTTPRequest::getBuffer(const char *buf, bool &multi)
{
	std::string line, input(buf);
	std::stringstream ss(input);
	std::vector<std::string> content;
	if (!multi)
	{
		takeHeader(ss);
		while (getline(ss, line))
			content.push_back(line + "\n");
		if (_last)
		{
			int count = 0;
			for (std::vector<std::string>::iterator it = content.begin(); it != content.end() && count < 4;)
			{
				if ((*it).find("filename") != std::string::npos)
					_fileName = (*it).substr((*it).find("filename") + 10, (*it).find_last_of("\"") - ((*it).find("filename") + 10) - 1);
				content.erase(it);
				count++;
			}
		}
		_boundary.append("--");
		checkBoundary(content);
	}
	else
	{
		while (getline(ss, line))
			content.push_back(line);
		checkBoundary(content);
	}
	content.clear();
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
