/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AdminServer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:26:56 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/31 12:26:50 by vduchi           ###   ########.fr       */
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

#include "HTTPRequestHandler.hpp"

class ServerConfig;

class AdminServer
{
private:
	bool _ref;	 // EV_FLAG0
	bool _multi; // EV_FLAG1
	bool _write;
	std::string _boundary;

	int _flags;
	ServerConfig _config;
	std::string _header;
	std::string _content;

public:
	AdminServer(const ServerConfig &config);
	~AdminServer();

	void run(int sockfd, int kq);
};

#endif
