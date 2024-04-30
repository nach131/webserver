/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AdminServer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:26:56 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/30 10:01:14 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#
#ifndef ADMINSERVER_HPP
#define ADMINSERVER_HPP

#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>

#include "ServerConfig.hpp"
#include "Colors.hpp"
#include "HTTPRes.hpp"
#include "HTTPRequest.hpp"
#include "HTTPHeader.hpp"
#include "WebServer.hpp"

class ServerConfig;

class AdminServer
{
private:
	ServerConfig _config;

public:
	AdminServer(const ServerConfig &config);
	~AdminServer();

	void run();
};

#endif
