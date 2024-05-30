/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AdminServer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:26:56 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/30 12:03:27 by nmota-bu         ###   ########.fr       */
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

// struct Run
// {
// 	struct kevent evSet;
// 	struct kevent evList[MAX_EVENTS];
// 	struct sockaddr_storage addr;
// 	socklen_t socklen = sizeof(addr);
// };

class AdminServer
{
private:
	// Run run;
	bool _ref;	 // EV_FLAG0
	bool _multi; // EV_FLAG1
	std::string _boundary;
	bool _multinach; // EV_FLAG1
	bool _write;

	int _flags;
	// bool _writeFault;
	LogFile _log;
	ServerConfig _config;
	std::string _header;
	std::string _headerNew;
	std::string _content;
	std::string _contentNew;

public:
	AdminServer(const ServerConfig &config);
	~AdminServer();

	void run(int sockfd, int kq);
};

#endif
