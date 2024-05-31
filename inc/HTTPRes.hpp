/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRes.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:54:32 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/31 19:58:38 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRES_HPP
#define HTTPRES_HPP

#include <iostream>
#include "Colors.hpp"
#include "PythonExec.hpp"
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
	Location _locationConf;
	bool _ref;
	bool &_write;
	bool &_send;

	std::string getRootLocate(const std::string &path) const;

	void rootPath();
	void rootOtherFiles();
	void last();
	void error404();
	void error403();
	// void error501();

	std::string const OtherPath() const;

	void locateOK(const std::string &path);
	void createContent(std::string filePath, bool file);
	void folder();

	void methodGet();
	void methodPost(const bool &autoindex);
	void methodDelete();
	void methodErr();

	void exploreFiles();
	void execUpload(const std::string &pathFile);

public:
	HTTPRes(const HTTPRequest &request, ServerConfig *config, const bool &ref, bool &write, bool &send);
	~HTTPRes();

	std::string const getHeader() const;
	std::string const getContent() const;
};

#endif
