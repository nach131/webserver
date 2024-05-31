/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AdminServer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:26:56 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/31 19:23:32 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADMINSERVER_HPP
#define ADMINSERVER_HPP

#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/event.h>

#include "ServerConfig.hpp"
#include "HTTPRes.hpp"
#include "HTTPRequest.hpp"
#include "HTTPHeader.hpp"
#include "LogFile.hpp"

#include "HTTPRequestHandler.hpp"

class ServerConfig;

class AdminServer
{
private:
	bool _ref;	 // EV_FLAG0
	bool _multi; // EV_FLAG1
	bool _write;
	bool _send;
	std::string _boundary;

	int _flags;
	LogFile _log;
	ServerConfig _config;
	std::string _header;
	std::string _content;

public:
	AdminServer(const ServerConfig &config);
	~AdminServer();

	void run(int sockfd, int kq);
};

#endif
