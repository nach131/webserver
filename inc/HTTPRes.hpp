/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRes.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:54:32 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/22 20:22:19 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRES_HPP
#define HTTPRES_HPP

#include <iostream>
#include <map>
#include "Colors.hpp"
#include "HTTPRequest.hpp"
#include "HTTPHeader.hpp"
#include "MIMETypeReader.hpp"
#include "WebServer.hpp"

class HTTPHeader;
class HTTPRequest;
class MIMETypeReader;

class HTTPRes
{
private:
	HTTPHeader _header;
	HTTPRequest _request;
	std::string _content;

public:
	HTTPRes(const HTTPRequest &request);
	~HTTPRes();

	std::string const getHeader() const;
	std::string const getContent() const;

	static void methodGet(HTTPRes &res);
	static void methodPost(HTTPRes &res);
	static void methodDelete(HTTPRes &res);
	static void methodErr(HTTPRes &res);
};

#endif
