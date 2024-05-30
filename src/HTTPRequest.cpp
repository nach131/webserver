/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:05:47 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/30 17:10:06 by vduchi           ###   ########.fr       */
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

std::string HTTPRequest::getFileType() { return _fileType; }

void HTTPRequest::findFileName(const char *buf)
{
	std::string input(buf);
	if (input.find("filename=") != std::string::npos)
	{
		_fileName = input.substr(input.find("filename=") + 10,
								 input.find_last_of("\"") - (input.find("filename=") + 10));
		_fileType = _fileName.substr(_fileName.find_last_of(".") + 1,
									 _fileName.length() - (_fileName.find(".") + 1) - 1);
		// std::cout << RED "Filename: -" << _fileName << "- FileType: -" << _fileType << "-" RESET << std::endl;
	}
}

void HTTPRequest::takeHeader(std::stringstream &ss, int &start)
{
	int i = 0;
	std::string first, el, line;
	std::getline(ss, first);
	start += first.length() + 1;
	// std::cout << MAGENTA "First length: " << first.length() << " Start: " << start << RESET << std::endl;
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
		start += line.length() + 1;
		// std::cout << MAGENTA "Line length: " << line.length() << " Start: " << start << RESET << std::endl;
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

void HTTPRequest::checkFirstBoundary(std::vector<std::string> &content, int &start)
{
	if (content[0].find(_boundary) == std::string::npos)
		return;
	int count = 0;
	for (std::vector<std::string>::iterator it = content.begin(); it != content.end() && count < 4;)
	{
		start += (*it).length();
		// std::cout << MAGENTA "First boundary length: " << (*it).length() << " Start: " << start << " Line: " << (*it) << RESET << std::endl;
		content.erase(it);
		count++;
	}
}

void HTTPRequest::checkLastBoundary(const char *buffer, int &start, int len)
{
	// for (std::vector<std::string>::iterator it = content.begin(); it != content.end();)
	// {
	// 	if ((*it).find(_boundary) != std::string::npos)
	// 	{
	// 		_last = false;
	// 		start += (*it).length();
	// 		content.erase(it);
	// 		if (it == content.end())
	// 			break;
	// 		continue;
	// 	}
	// 	it++;
	// }
	// std::string mapContent;
	// for (std::vector<std::string>::iterator it = content.begin(); it != content.end(); it++)
	// 	mapContent += *it;
	// _map["Content"] = mapContent;
	bool check;
	for (int i = start; i < len; i++)
	{
		check = true;
		if (buffer[i] == '-')
		{
			for (int j = i; j < i + 8; j++)
			{
				// std::cout << ORANGE "J CHAR: " << buffer[j] << " J: " << j << RESET << std::endl;
				if (buffer[j] != '-')
				{
					check = false;
					break;
				}
			}
			if (!check)
				continue;
			for (int j = i; buffer[j] != '\0'; j++)
			{
				start++;
				// std::cout << MAGENTA "Char last boundary: " << buffer[j] << " Start: " << start << RESET << std::endl;
			}
			break;
		}
	}
}

void HTTPRequest::getBuffer(const char *buf, int len, bool &multi)
{
	int i;
	int start;
	// std::ifstream in(buf, std::ios::in | std::ios::binary);
	// std::stringstream in(buf);
	std::vector<std::string> content;
	// unsigned int mySize = len;
	// out.write(reinterpret_cast<char *>(&mySize), sizeof(unsigned int));
	// out.write(buf, mySize * sizeof(char));
	std::string line, input(buf);
	std::stringstream ss(input);
	if (!multi)
	{
		start = 0;
		findFileName(buf);
		takeHeader(ss, start);
		// std::cout << RED << "Print header:\n";
		// for (std::map<std::string, std::string>::iterator it = _map.begin(); it != _map.end(); it++)
		// 	std::cout << "Key: " << it->first << " Value: " << it->second << std::endl;
		// std::cout << RESET << std::endl;
		while (getline(ss, line))
			content.push_back(line + "\n");
		if (content.size() > 0)
		{
			if (_last)
				checkFirstBoundary(content, start);
			_boundary.append("--");
			checkLastBoundary(buf, start, len);
		}
		std::ofstream out("conf_web/out", std::ios::app | std::ios::binary);
		if (!out.good())
		{
			// std::cout << "Out bad" << std::endl;
			exit(0);
		}
		for (i = 0; i < len; i++)
			;
		// std::cout << "\nBuf length: ";
		// std::cout << i << " Len: " << len << std::endl
		//   << std::endl;
		// std::cout << "No Multi Written characters: " << i - start << " I:" << i << " Start: " << start << std::endl;
		out.write(&buf[start], i - start);
		out.close();
		setMulti(multi);
	}
	else
	{
		start = 0;
		// std::cout << RED << "Print header:\n";
		// for (std::map<std::string, std::string>::iterator it = _map.begin(); it != _map.end(); it++)
		// 	std::cout << "Key: " << it->first << " Value: " << it->second << std::endl;
		// std::cout << RESET << std::endl;
		while (getline(ss, line))
			content.push_back(line + "\n");
		checkFirstBoundary(content, start);
		checkLastBoundary(buf, start, len);
		std::ofstream out("conf_web/out", std::ios::app | std::ios::binary);
		if (!out.good())
		{
			// std::cout << "Out bad" << std::endl;
			exit(0);
		}
		for (i = 0; i < len; i++)
			;
		// std::cout << "Multi Written characters: " << i - start << " I:" << i << " Start: " << start << std::endl;
		out.write(buf, i - start);
		out.close();
		setMulti(multi);
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
