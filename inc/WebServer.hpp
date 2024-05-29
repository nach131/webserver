/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:34:48 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/29 19:33:24 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_H__
#define WEBSERVER_H__

#include "Colors.hpp"
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <vector>
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

#endif
