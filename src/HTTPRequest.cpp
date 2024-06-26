/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:05:47 by vduchi            #+#    #+#             */
/*   Updated: 2024/06/02 13:38:02 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"
#include "WebServer.hpp"

HTTPRequest::HTTPRequest() : _last(false), _firstBound(false)
{
	_fileName = "";
	_fileType = "";
	_boundary = "";
}

HTTPRequest::HTTPRequest(const HTTPRequest &rhs)
{
	_last = rhs._last;
	_firstBound = rhs._firstBound;
	_fileName = rhs._fileName;
	_fileType = rhs._fileType;
	_boundary = rhs._boundary;
	_map = rhs._map;
}

HTTPRequest::~HTTPRequest() {}

void HTTPRequest::setMulti(bool &multi) { multi = _last; }

std::string HTTPRequest::getFileName() { return _fileName; }

std::string HTTPRequest::getFileType() { return _fileType; }

const std::map<std::string, std::string> &HTTPRequest::getMap() const { return _map; }

const std::string &HTTPRequest::getHeader(const std::string &name) const
{
	std::map<std::string, std::string>::const_iterator it = _map.find(name);

	if (it != _map.end())
		return it->second;

	static const std::string emptyString;
	return emptyString;
}

std::string HTTPRequest::getLocation()
{
	std::string path = getHeader("Path");
	if (path == "/" || path == "/index.html")
		return "/";
	std::vector<std::string> parts = split(path, '/');
	return "/" + parts[0];
}

void HTTPRequest::findFileName(const char *buf)
{
	std::string input(buf);
	if (input.find("filename=") != std::string::npos)
	{
		unsigned long start = input[input.find("filename=") + 9] == '\"' ? input.find("filename=") + 10 : input.find("filename=") + 9;
		unsigned long end = start == input.find("filename=") + 10 ? input.find_first_of("\"", input.find("filename=") + 11) : input.find_first_of("\r", input.find("filename=") + 9);
		_fileName = input.substr(start, end - start);
		_fileType = _fileName.substr(_fileName.find_last_of(".") + 1,
									 _fileName.length() - (_fileName.find(".") + 1));
	}
}

void HTTPRequest::takeHeader(std::stringstream &ss, int &start)
{
	int i = 0;
	std::string first, el, line;
	std::getline(ss, first);
	start += first.length() + 1;
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
	_firstBound = true;
	for (std::vector<std::string>::iterator it = content.begin(); it != content.end() && count < 4;)
	{
		start += (*it).length();
		content.erase(it);
		count++;
	}
}

void HTTPRequest::checkLastBoundary(const char *buffer, int &start, int &end, int len)
{
	if (_last)
	{
		bool check;
		for (int i = start; i < len; i++)
		{
			check = true;
			if (buffer[i] == '-')
			{
				for (int j = i; j < i + 8; j++)
				{
					if (buffer[j] != '-')
					{
						check = false;
						break;
					}
				}
				if (!check)
					continue;
				_last = false;
				for (int j = i; buffer[j] != '\0'; j++)
					end++;
				end++;
				break;
			}
		}
	}
}

void HTTPRequest::takeBuffer(const char *buf, int len, bool &multi, bool &write)
{
	int end;
	int start;
	std::string line, input(buf);
	std::stringstream ss(input);
	std::vector<std::string> content;
	if (!multi)
	{
		std::system("rm ./.tmpdir/.bin 2>/dev/null");
		end = 0;
		start = 0;
		findFileName(buf);
		takeHeader(ss, start);
		while (getline(ss, line))
			content.push_back(line + "\n");
		if (content.size() > 0 && _last)
		{
			checkFirstBoundary(content, start);
			_boundary.append("--");
			checkLastBoundary(buf, start, end, len);
			std::ofstream out(".tmpdir/.bin", std::ios::app | std::ios::binary);
			if (!out.good())
				Log::error(REQ, "Temporary file not opened!");
			out.write(&buf[start], len - start - end);
			out.close();
		}
		else if (content.size() > 0 && !_last)
		{
			std::string mapContent;
			for (std::vector<std::string>::iterator it = content.begin(); it != content.end(); it++)
				mapContent += *it;
			_map["Content"] = mapContent;
		}
		setMulti(multi);
		content.clear();
	}
	else
	{
		end = 0;
		start = 0;
		while (getline(ss, line))
			content.push_back(line + "\n");
		if (!_firstBound)
			checkFirstBoundary(content, start);
		checkLastBoundary(buf, start, end, len);
		std::ofstream out(".tmpdir/.bin", std::ios::app | std::ios::binary);
		if (!out.good())
			Log::error(REQ, "Temporary file not opened!");
		out.write(&buf[start], len - start - end);
		out.close();
		setMulti(multi);
	}
	if (!multi)
	{
		_firstBound = false;
		write = true;
	}
}

void HTTPRequest::cleanObject()
{
	_map.clear();
	_boundary.clear();
	_fileName.clear();
	_fileType.clear();
}
