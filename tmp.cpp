#ifndef HTTPREQUESTHANDLER_HPP
#define HTTPREQUESTHANDLER_HPP

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>

#define MAX_MSG_SIZE 1024

class HTTPRequestHandler
{
public:
	HTTPRequestHandler(char buffer[MAX_MSG_SIZE], bool &multi, bool &write, std::string &boundary);
	~HTTPRequestHandler();

	std::string const getContent() const;
	void print() const;

private:
	void processRequest(char buffer[MAX_MSG_SIZE], bool &multi, bool &write, std::string &boundary);

	bool _multi;
	bool _write;
	std::map<std::string, std::string> _map;
	std::string _content;
};

#endif

HTTPRequestHandler::HTTPRequestHandler(char buffer[MAX_MSG_SIZE], bool &multi, bool &write, std::string &boundary)
{
	processRequest(buffer, multi, write, boundary);
}

HTTPRequestHandler::~HTTPRequestHandler() {}

std::string const HTTPRequestHandler::getContent() const { return _content; }

void HTTPRequestHandler::print() const
{
	for (std::map<std::string, std::string>::const_iterator it = _map.begin(); it != _map.end(); ++it)
	{
		std::cout << it->first << ": " << it->second << std::endl;
	}
	// if (_multi)
	// std::cout << "Content: " << _content << std::endl;
}

void HTTPRequestHandler::processRequest(char buffer[MAX_MSG_SIZE], bool &multi, bool &write, std::string &boundary)
{
	std::istringstream stream(buffer);
	std::string line;

	// Parse HTTP headers
	if (!multi)
	{
		while (std::getline(stream, line) && line != "\r")
		{
			std::string::size_type pos = line.find(": ");
			if (pos != std::string::npos)
			{
				std::string key = line.substr(0, pos);
				std::string value = line.substr(pos + 2);
				value.erase(value.find_last_not_of("\r\n") + 1);
				_map[key] = value;

				// Check for multipart
				if (key == "Content-Type" && value.find("multipart/form-data") != std::string::npos)
				{
					multi = true;
				}
			}
		}
	}

	if (multi)
	{
		if (_map.find("Content-Type") != _map.end())
			boundary = "--" + _map["Content-Type"].substr(_map["Content-Type"].find("boundary=") + 9);

		bool boundaryEndFound = false;
		while (std::getline(stream, line))
		{
			if (line.find(boundary) != std::string::npos)
			{
				if (line.find(boundary + "--") != std::string::npos)
				{
					boundaryEndFound = true;
					break;
				}

				// Skip the header lines for the part
				while (std::getline(stream, line) && line != "\r")
				{
				}

				// Read the content of the part
				std::ostringstream partContent;
				while (std::getline(stream, line) && line.find(boundary) == std::string::npos)
				{
					partContent << line << "\n";
				}

				// Handle binary data
				std::string partData = partContent.str();
				std::size_t endPos = partData.find_last_of('\n');
				if (endPos != std::string::npos && endPos == partData.size() - 1)
					partData.erase(endPos);

				_content.append(partData.begin(), partData.end());
			}
		}

		if (boundaryEndFound)
			write = true;
	}
}
