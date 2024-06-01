/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:34:48 by vduchi            #+#    #+#             */
/*   Updated: 2024/06/01 11:45:36 by vduchi           ###   ########.fr       */
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
#include <sys/stat.h>

#define MAX_EVENTS 32
#define MAX_MSG_SIZE 8192

int start();

std::string DateField();
std::string generateETag(const std::string &content);
std::string readFile(const std::string &filePath);
std::string getExtension(const std::string &path);
std::string extractEndpoint(const std::string &url);
bool isFile(const std::string &path);
bool directoryExists(const std::string &path);
std::string getFileName(const std::string &path);
std::vector<std::string> split(const std::string &str, char delimiter);
bool isMainRoot(const std::string &str);
void removeLastSlash(std::string &str);
void removeLastNewline(std::string &str);
void removeDoubleSlashes(std::string &path);
std::string removeBeforeNumber(const std::string &url, const std::string &host);
std::string removeFileName(const std::string &path);
void takeOutSemiColumn(std::string &val);
bool parseError(std::string str, int n);
std::string removeSubstring(const std::string &str, const std::string &toRemove);
std::string rmLocationToAPath(const std::string &path, const std::string &location);
bool pathStartsWithLocation(const std::string &path, const std::string &location);

bool createDirectory(const std::string &path);
bool isText(const std::string &str);
std::string intToString(int number);

// enum
// {
// 	CONN,	// Connection error
// 	RECV,	// Receive data error
// 	FOPEN,	// File open error
// 	PGERR,	// Page error
// 	EXERR,	// Execution error
// 	DISC,	// Client disconnected
// 	GET,	// Get Method
// 	POST,	// Post Method
// 	DELETE, // Delete method
// };

enum
{
	ADM,
	REQ,
	RESP,
	GET,
	POST,
	DEL
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
	static void writeHtml(const std::string &value);
};

#endif
