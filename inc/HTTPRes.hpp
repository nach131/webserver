/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRes.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:54:32 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/02 19:40:05 by nmota-bu         ###   ########.fr       */
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
    std::map<std::string, std::map<std::string, std::string> >  _locations; 

	bool isUrlAllowed(const std::string& url) const ;
	void rootPath();
	void rootOtherFiles();
	void last();
void error404();

	std::string const OtherPath() const;

public:
	HTTPRes(const HTTPRequest &request, ServerConfig *config);
	~HTTPRes();

	std::string const getHeader() const;
	std::string const getContent() const;


void methodGet();
void methodPost();
void methodDelete();
void methodErr();

// bool isUrlAllowed(const std::string& url, const std::string& method) const ;





};

#endif
