/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPBody.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 12:12:11 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/16 17:41:41 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPBody_HPP
#define HTTPBody_HPP

#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include "ClientParsing.hpp"

class HTTPBody
{
private:
	std::string _content;
	std::string _code;
	HTTPBody &operator=(const HTTPBody &);

	HTTPBody(const HTTPBody &);

	static int methodGet(HTTPBody &client, std::string path);
	static int methodPost(HTTPBody &client, std::string path);
	static int methodDelete(HTTPBody &client, std::string path);

public:
	HTTPBody(const ClientParsing &);

	std::string const &content() const;
	std::string const &code() const;

	~HTTPBody();
};

#endif
