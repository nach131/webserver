/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPHeader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:24:48 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/22 15:56:43 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPHeader.hpp"

HTTPHeader::HTTPHeader() {}

HTTPHeader::~HTTPHeader() {}

HTTPHeader::HTTPHeader(const std::string &prot, const std::string &code)
{
	_header = prot + " " + code + "\r\n";
}

void HTTPHeader::addOne(const std::string &prot, const std::string &code)
{
	_header = prot + " " + code + "\r\n";
}

void HTTPHeader::addField(const std::string &key, const std::string &value)
{
	_header += key + ": " + value + "\r\n";
}

std::string const HTTPHeader::getHeader() const
{
	std::string header = _header + "\r\n";
	return header;
}

size_t HTTPHeader::headerLength() const { return _header.length(); }
