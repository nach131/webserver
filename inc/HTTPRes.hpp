/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRes.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:54:32 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/15 14:08:51 by nmota-bu         ###   ########.fr       */
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
#include "ServerConfig.hpp"

class HTTPHeader;
class HTTPRequest;
class ServerConfig;

class HTTPRes
{
private:
	HTTPHeader _header;
	HTTPRequest _request;
	ServerConfig *_config;
	std::string _content;
    // std::map<std::string, std::map<std::string, std::string> >  _locations;

	// bool isMethodAllowed(const std::string &path, const std::string &method) const;
	std::string getRootLocate(const std::string &path) const;

	void rootPath();
	void rootOtherFiles();
	void last();
void error404();

	std::string const OtherPath() const;

	void locateOK(const std::string &path);

public:
	HTTPRes(const HTTPRequest &request, ServerConfig *config);
	~HTTPRes();

	std::string const getHeader() const;
	std::string const getContent() const;


void methodGet();
void methodPost();
void methodDelete();
void methodErr();

void exploreFiles();

// bool isUrlAllowed(const std::string& url, const std::string& method) const ;





};

#endif
