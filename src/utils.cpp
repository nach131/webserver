/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:48:00 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/06/02 13:34:41 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

std::string DateField()
{
	time_t currentTime;
	time(&currentTime);
	struct tm *timeInfo = gmtime(&currentTime);
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", timeInfo);
	return buffer;
}

std::string generateETag(const std::string &content)
{
	std::hash<std::string> hasher;
	size_t hashValue = hasher(content);
	std::stringstream ss;
	ss << std::hex << hashValue;
	return ss.str();
}

std::string readFile(const std::string &filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open())
		return "";
	std::ostringstream oss;
	oss << file.rdbuf();
	return oss.str();
}

std::string getExtension(const std::string &path)
{
	std::string extension;
	size_t pos = path.find_last_of('.');
	return path.substr(pos + 1);
}

bool isFile(const std::string &path)
{
	std::string adjustedPath = path;
	if (!path.empty() && path[0] == '.')
		adjustedPath = path.substr(1);
	std::size_t found = adjustedPath.find_last_of(".");
	return (found != std::string::npos && found != path.size() - 1);
}

bool isFileExist(const std::string &path)
{
	struct stat statbuf;
	if (stat(path.c_str(), &statbuf) != 0)
		return false;
	return S_ISREG(statbuf.st_mode);
}

bool directoryExists(const std::string &path)
{
	struct stat info;
	if (stat(path.c_str(), &info) != 0)
		return false;
	else if (info.st_mode & S_IFDIR)
		return true;
	else
		return false;
}

std::string getFileName(const std::string &path)
{
	std::size_t pos = path.find_last_of('/');
	if (pos != std::string::npos)
		return path.substr(pos + 1);
	return path;
}

std::vector<std::string> split(const std::string &str, char delimiter)
{
	std::vector<std::string> tokens;
	std::istringstream stream(str);
	std::string token;
	while (std::getline(stream, token, delimiter))
	{
		if (!token.empty())
			tokens.push_back(token);
	}
	return tokens;
}

void removeLastSlash(std::string &str)
{
	if (!str.empty() && str[str.length() - 1] == '/')
		str.erase(str.length() - 1);
}

void removeLastNewline(std::string &str)
{
	if (!str.empty() && str[str.length() - 1] == '\n')
		str.erase(str.length() - 1);
}
void removeLastReturn(std::string &str)
{
	if (!str.empty() && str[str.length() - 1] == '\r')
		str.erase(str.length() - 1);
}

void removeDoubleSlashes(std::string &path)
{
	std::string::size_type pos = 0;
	while ((pos = path.find("//", pos)) != std::string::npos)
	{
		path.erase(pos, 1);
	}
}

std::string removeBeforeNumber(const std::string &url, const std::string &host)
{
	std::string result = url;
	std::size_t pos = result.find(host);
	if (pos != std::string::npos)
	{
		pos = result.find("/", pos);
		if (pos != std::string::npos)
			result = result.substr(pos);
		else
			result = "";
		if (!result.empty() && result.back() == '\r')
			result.erase(result.size() - 1);
	}
	return result;
}

void takeOutSemiColumn(std::string &val)
{
	size_t i = 0;
	size_t j = val.length() - 1;
	if (!val.length())
		return;
	for (; i < val.length(); i++)
		if (val[i] == ';')
			break;
	for (; j >= i; j--)
		val.pop_back();
}

std::string removeSubstring(const std::string &str, const std::string &toRemove)
{
	std::string result = str;
	size_t pos = 0;
	while ((pos = result.find(toRemove, pos)) != std::string::npos)
		result.erase(pos, toRemove.length());

	return result;
}

bool pathStartsWithLocation(const std::string &path, const std::string &location)
{
	return path.compare(0, location.length(), location) == 0;
}

bool createDirectory(const std::string &path)
{
	int status = mkdir(path.c_str(), 0755);
	if (status == 0)
		return true;
	else
	{
		if (errno == EEXIST)
			return true;
		else
			return false;
	}
}

std::string intToString(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

std::string generateToken(size_t length)
{
	const char hex_chars[] = "0123456789abcdef";
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 15);

	std::stringstream ss;
	for (size_t i = 0; i < length; ++i)
	{
		ss << hex_chars[dis(gen)];
	}
	return ss.str();
}

bool isValidToken(const std::string &token, size_t expectedLength)
{
	if (token.length() != expectedLength)
		return false;
	const std::regex hexPattern("^[0-9a-f]+$");
	if (!std::regex_match(token, hexPattern))
		return false;
	return true;
}
