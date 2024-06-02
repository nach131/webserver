/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AdminServer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:26:56 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/06/02 13:10:13 by vduchi           ###   ########.fr       */
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

class ServerConfig;

class AdminServer
{
private:
	bool _ref;
	bool _multi;
	bool _write;
	bool _send;
	std::string _boundary;

	int _flags;
	std::string _header;
	std::string _content;
	std::vector<ServerConfig *> _config;

public:
	AdminServer(std::vector<ServerConfig *> &);
	~AdminServer();

	void run(int kq);
	int takeServer(struct kevent &);
};

#endif
