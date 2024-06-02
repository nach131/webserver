/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileContent.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:32:24 by vduchi            #+#    #+#             */
/*   Updated: 2024/06/02 12:53:41 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileContent.hpp"

FileContent::FileContent() {}

void FileContent::openFile(const char *file)
{
	std::string line;
	std::ifstream in(file);
	if (!in.good())
		throw std::runtime_error("configuration file not opened correctly!");
	while (getline(in, line))
		_content.push_back(line);
}

int FileContent::checkLine(std::string &line)
{
	size_t l = 0;
	while (line[l] == ' ' || line[l] == '\t')
		l++;
	if (line[l] != '#' && line[l] != '\0')
		return 1;
	return 0;
}

int FileContent::checkServerKey(std::string &line)
{
	size_t i = line.find("server");
	if (i == std::string::npos ||
		(i != std::string::npos && line.find("name") != std::string::npos))
		return 0;
	if (i == 0)
		return 1;
	for (; i >= 0; i--)
		if (line[i] != ' ' && line[i] != '\t')
			return 0;
	return 1;
}

void FileContent::createOneServer(std::vector<ServerConfig *> &servers, std::vector<std::string> &content, int len)
{
	ServerConfig *sc = new ServerConfig(content, "./conf_web/mime.type", len);
	servers.push_back(sc);
}

void FileContent::checkKeysAndValues(std::vector<std::string> &content, int &start)
{
	KeyValue kv;
	for (size_t i = 0; i < content.size(); i++)
	{
		if (checkServerKey(content[i]))
			continue;
		if (content[i].find("location") != std::string::npos)
		{
			std::string el;
			std::stringstream ss(content[i]);
			ss >> el >> el;
			if (el.length() == 0 || el.find(";") != std::string::npos ||
				el.find("{") != std::string::npos || el.find("/") == std::string::npos ||
				el.find("/") != 0 || el.find_first_of('/') != el.find_last_of('/'))
				error("location not declared correctly at line ", start + i + 1);
		}
		else if (checkLine(content[i]) &&
				 content[i].find("{") == std::string::npos &&
				 content[i].find("}") == std::string::npos)
		{
			std::string key, value;
			std::string el;
			std::stringstream ss(content[i]);
			ss >> key;
			ss >> value;
			if (value.length() > 0 && value.find(";") == std::string::npos)
			{
				while (ss >> el)
				{
					value.append(" " + el);
					if (value.find(";") != std::string::npos)
						break;
				}
			}
			if (value.length() > 0)
				value.erase(value.find(";"), value.find("0"));
			if (key == "error_page" || key == "allow_methods")
				kv.checkComplex(key, value, start + i + 1);
			else if (kv.checkKey(key))
				error("key is not allowed at line ", start + i + 1);
			else if (kv.checkValue(key, value))
				error("value is not acceptable for key at line ", start + i + 1);
		}
	}
}

std::vector<std::string> FileContent::checkOneServer(int &start, int &len)
{
	int brack = 0;
	std::vector<std::string> sCont;
	for (int i = start; i <= start + len; i++)
		sCont.push_back(_content[i]);

	// Test for curly brackets
	for (size_t i = 0; i < sCont.size(); i++)
	{
		if (sCont[i].find("{") != std::string::npos)
			brack++;
		if (sCont[i].find("}") != std::string::npos)
			brack--;
	}
	int lNum = sCont[sCont.size() - 1].find("}") != std::string::npos ? start + len + 1 : start + len;
	if (brack > 0)
		error("server block not closed at line ", lNum + 1);
	else if (brack < 0)
		error("expected block before } at line ", lNum);

	// Test for semicolumn
	for (size_t i = 0; i < sCont.size(); i++)
	{
		if (checkLine(sCont[i]) &&
			(sCont[i].find("server") == std::string::npos ||
			 (sCont[i].find("server") != std::string::npos &&
			  sCont[i].find("name") != std::string::npos)) &&
			sCont[i].find("location") == std::string::npos &&
			sCont[i].find("{") == std::string::npos &&
			sCont[i].find("}") == std::string::npos &&
			sCont[i].find(";") == std::string::npos)
			error("expected semicolumn at the end of the line ", start + i + 1);
	}
	checkKeysAndValues(sCont, start);
	return sCont;
}

void FileContent::createServers(std::vector<ServerConfig *> &servers)
{
	int start = 0;
	int count = 1;
	std::vector<std::string> oneServer;
	for (size_t i = 0; i < _content.size(); i++)
	{
		if (checkServerKey(_content[i]))
		{
			if (count == 1)
				start = i;
			count--;
		}
		if (count == -1 || i == _content.size() - 1)
		{
			int diff = i == _content.size() - 1 ? i - start : i - start - 1;
			oneServer = checkOneServer(start, diff);
			createOneServer(servers, oneServer, start);
			checkDuplicatePorts(servers);
			start = i;
			count++;
		}
	}
}

void FileContent::checkDuplicatePorts(std::vector<ServerConfig *> &servers)
{
	std::vector<int> ports;
	for (std::vector<ServerConfig *>::iterator it = servers.begin(); it != servers.end(); it++)
		ports.push_back((*it)->getPort());
	for (std::vector<int>::iterator it = ports.begin(); it != ports.end(); it++)
	{
		for (std::vector<int>::iterator it2 = ports.begin(); it2 != ports.end(); it2++)
		{
			if (it2 != it && *it == *it2)
				throw std::runtime_error("more than one server with the same port!");
		}
	}
}

void FileContent::error(const std::string msg, int lineNum)
{
	std::stringstream ss;
	ss << msg << lineNum;
	throw e_cee(ss.str());
}

FileContent::ConfErrorException::ConfErrorException(const std::string &msg)
	: std::runtime_error("\033[0;31mError: " + msg) {}

FileContent::~FileContent() { _content.clear(); }
