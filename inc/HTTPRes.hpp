/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRes.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:54:32 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/21 12:31:41 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRES_HPP
#define HTTPRES_HPP

#include <iostream>
#include <map>
#include "Colors.hpp"
#include "HTTPRequest.hpp"
#include "HTTPHeader.hpp"
#include "WebServer.hpp"
#include "Location.hpp"
#include "ServerConfig.hpp"

class HTTPHeader;
class HTTPRequest;
class ServerConfig;
class Location;

class HTTPRes
{
private:
	HTTPHeader _header;
	std::string _content;

	HTTPRequest _request;
	ServerConfig *_config;
	// std::string _location;
	Location _locationConf;

	std::string getRootLocate(const std::string &path) const;

	void rootPath();
	void rootOtherFiles();
	void last();
	void error404();
	void error403();

	std::string const OtherPath() const;

	void locateOK(const std::string &path);
	void createContent(std::string filePath, bool file);
	void folder();
	std::string execPython(const std::string &filePath);

	void methodGet();

	// void file();
	void exploreFiles();

public:
	HTTPRes(const HTTPRequest &request, ServerConfig *config);
	~HTTPRes();

	std::string const getHeader() const;
	std::string const getContent() const;

	void methodPost();
	void methodDelete();
	void methodErr();

	// bool isUrlAllowed(const std::string& url, const std::string& method) const ;
};

#endif
