/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:34:48 by vduchi            #+#    #+#             */
/*   Updated: 2024/06/02 13:30:36 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_H__
#define WEBSERVER_H__

#include "Colors.hpp"
#include <map>
#include <ctime>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>
#include <vector>
#include <random>
#include <regex>
#include <sys/stat.h>

#define MAX_EVENTS 32
#define MAX_MSG_SIZE 8192

std::string DateField();
std::string generateETag(const std::string &content);
std::string readFile(const std::string &filePath);
std::string getExtension(const std::string &path);
bool isFile(const std::string &path);
bool isFileExist(const std::string &path);
bool directoryExists(const std::string &path);
std::string getFileName(const std::string &path);
std::vector<std::string> split(const std::string &str, char delimiter);

void removeLastSlash(std::string &str);
void removeLastNewline(std::string &str);
void removeLastReturn(std::string &str);

void removeDoubleSlashes(std::string &path);
std::string removeBeforeNumber(const std::string &url, const std::string &host);
void takeOutSemiColumn(std::string &val);
std::string removeSubstring(const std::string &str, const std::string &toRemove);
bool pathStartsWithLocation(const std::string &path, const std::string &location);

bool createDirectory(const std::string &path);
std::string intToString(int number);
std::map<std::string, std::string> parse_query_string(const std::string &query);
std::string generateToken(size_t length);
bool isValidToken(const std::string &token, size_t expectedLength);

enum
{
	ADM,
	REQ,
	RESP,
	GET,
	POST,
	DEL,
	PY
};

class Log
{
private:
	Log();
	~Log();
	std::fstream file;
	static Log instance;

public:
	static void info(int, std::string);
	static void warn(int, std::string);
	static void error(int, std::string);
};

#endif
