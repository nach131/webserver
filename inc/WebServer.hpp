/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:34:48 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/21 12:19:59 by vduchi           ###   ########.fr       */
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

#define MAX_EVENTS 32
#define MAX_MSG_SIZE 8192

int start();

std::string DateField();
std::string generateETag(const std::string &content);
std::string readFile(const std::string &filePath);
std::string getExtension(const std::string &path);
std::string extractEndpoint(const std::string &url);
bool isFile(const std::string &path);
std::vector<std::string> split(const std::string &str, char delimiter);
bool isMainRoot(const std::string &str);
void removeLastSlash(std::string &str);
std::string removeBeforeNumber(const std::string &url, const std::string &host);
std::string removeFileName(const std::string &path);
void takeOutSemiColumn(std::string &val);
bool parseError(std::string str, int n);

#endif
