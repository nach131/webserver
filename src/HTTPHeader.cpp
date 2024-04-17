/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPHeader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:24:48 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/17 12:30:34 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPHeader.hpp"

HTTPHeader::HTTPHeader(const std::string &prot, const std::string &code)
{
	std::string tmp = prot + " " + code + "\r\n";
	_header = tmp;
	addField("Date", DateField());
}

HTTPHeader::~HTTPHeader() {}

std::string HTTPHeader::DateField() const
{
	// Obtener la fecha y hora actual
	time_t currentTime;
	time(&currentTime);
	struct tm *timeInfo = gmtime(&currentTime);

	// Formatear la fecha según el estándar RFC 1123 (ejemplo: "Tue, 15 Nov 1994 08:12:31 GMT")
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", timeInfo);
	return buffer;
}

void HTTPHeader::addField(const std::string &key, const std::string &value)
{
	_fields[key] = value;
}

std::string HTTPHeader::getHeader() const
{
	std::string header = _header;

	for (FieldMap::const_iterator it = _fields.begin(); it != _fields.end(); ++it)
		header += it->first + ": " + it->second + "\r\n";
	header += "\r\n";
	return header;
}

size_t HTTPHeader::headerLength() const { return _header.length(); }
