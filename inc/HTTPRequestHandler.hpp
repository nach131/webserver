/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestHandler.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 09:57:07 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/30 12:47:06 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTHANDLER_HPP
#define HTTPREQUESTHANDLER_HPP

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <cstring>

#include "WebServer.hpp"

class HTTPRequestHandler
{

private:
	std::string _content;
	std::map<std::string, std::string> _map;

	void processRequest(char buffer[MAX_MSG_SIZE], bool &multi, bool &write, std::string &boundary);

public:
	HTTPRequestHandler(char buffer[MAX_MSG_SIZE], bool &multi, bool &write, std::string &boundarys);
	~HTTPRequestHandler();

	void print() const;
	std::string const getCount() const;
};

#endif
