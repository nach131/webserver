/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:34:48 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/16 10:49:06 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_H__
# define WEBSERVER_H__

# include "Colors.hpp"
# include <map>
# include <fstream>
# include <iostream>
# include <sstream>
# include <functional>

#define MAX_EVENTS 32
#define MAX_MSG_SIZE 8192

int start();

std::string DateField();
std::string generateETag(const std::string &content);
std::string readFile(const std::string &filePath);
std::string getExtension(const std::string &path);
std::string extractEndpoint(const std::string &url);
bool isFile(const std::string& path);
std::vector<std::string> split(const std::string &str, char delimiter);

#endif
