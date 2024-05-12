/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:34:48 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/01 21:11:16 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_H__
# define WEBSERVER_H__

# include "Colors.hpp"
# include <map>
# include <fstream>
#include <sstream>
#include <csignal>
#include <iostream>
# include <functional>

int start();

std::string DateField();
std::string generateETag(const std::string &content);
std::string readFile(const std::string &filePath);
std::string getExtension(const std::string &path);
std::string extractEndpoint(const std::string &url);
bool isFile(const std::string& path);

#endif
