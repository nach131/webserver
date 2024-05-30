/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestHandler.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 09:57:07 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/30 11:29:28 by nmota-bu         ###   ########.fr       */
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
	std::map<std::string, std::string> _map;
	std::string _content;

	void processRequest(char buffer[MAX_MSG_SIZE], bool &multi, bool &write, std::string &boundary);

public:
	HTTPRequestHandler(char buffer[MAX_MSG_SIZE], bool &multi, bool &write, std::string &boundarys);
	~HTTPRequestHandler();
	void print() const;
	std::string const getCount() const;
};

#endif
