/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRes.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:54:32 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/27 17:38:39 by nmota-bu         ###   ########.fr       */
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
#include <cstdlib>

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
	bool _ref;

	std::string getRootLocate(const std::string &path) const;
	std::string pyExplorer(const std::string &py, const std::string &dirPath, const std::string &root_location);
	std::string pyExplorer(const std::string &py, const std::string &data);

	std::string execPython(const std::string &filePath);
	std::string execPython(const std::string &filePath, const std::string &realBuffer);

	void rootPath();
	void rootOtherFiles();
	void last();
	void error404();
	void error403();

	std::string const OtherPath() const;

	void locateOK(const std::string &path);
	void createContent(std::string filePath, bool file);
	void folder();

	void methodGet();
	void methodPost(const bool &autoindex);
	void methodDelete();
	void methodErr();

	// void file();
	void exploreFiles();

public:
	HTTPRes(const HTTPRequest &request, ServerConfig *config, const bool &ref);
	~HTTPRes();

	std::string const getHeader() const;
	std::string const getContent() const;

	// bool isUrlAllowed(const std::string& url, const std::string& method) const ;
};

#endif
