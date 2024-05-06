/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRes.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:54:32 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/03 19:38:32 by nmota-bu         ###   ########.fr       */
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
	ServerConfig _config;
	std::string _content;
    std::map<std::string, std::map<std::string, std::string> >  _locations; 

public:
	HTTPRes(const HTTPRequest &request, const ServerConfig &config);
	~HTTPRes();

	std::string const getHeader() const;
	std::string const getContent() const;

	static void methodGet(HTTPRes &res);
	static void methodPost(HTTPRes &res);
	static void methodDelete(HTTPRes &res);
	static void methodErr(HTTPRes &res);

// bool isUrlAllowed(const std::string& url, const std::string& method) const ;

bool isUrlAllowed(const std::string& url) const ;
// void rootPath(HTTPRes &res);
// void rootOtherFiles();

};

#endif
