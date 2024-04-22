/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPBody.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 12:12:11 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/22 00:16:52 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPBody_HPP
#define HTTPBody_HPP

#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include "HTTPRequest.hpp"

class HTTPBody
{
private:
	std::string _content;
	std::string _code;
	std::string _mime;
	HTTPBody &operator=(const HTTPBody &);

	HTTPBody(const HTTPBody &);

	static int methodGet(HTTPBody &client, std::string path);
	static int methodPost(HTTPBody &client, std::string path);
	static int methodDelete(HTTPBody &client, std::string path);

public:
	HTTPBody(const HTTPRequest &);

	std::string const &content() const;
	std::string const &code() const;
	std::string const &mime() const;
	void setMIME(const std::string &mime);

	~HTTPBody();
};

#endif
