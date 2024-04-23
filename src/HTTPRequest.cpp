/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:05:47 by vduchi            #+#    #+#             */
/*   Updated: 2024/04/23 13:20:57 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"

HTTPRequest::HTTPRequest(const char *buf)
{
	int i = 0;
	std::string first, el, line, input(buf);
	std::stringstream ss(input);
	std::getline(ss, first);
	std::stringstream ssFirst(first);

	while (std::getline(ssFirst, el, ' '))
	{
		if (!el.empty() && el.back() == '\r')
			el.pop_back();
		switch (i)
		{
		case 0:
			_map["Method"] = el;
			break;
		case 1:

			_map["Path"] = el;
			break;
		case 2:
			_map["Version"] = el;
			break;
		}
		i++;
	}

	while (std::getline(ss, line))
	{
		if (line.length() == 1)
			break; // Headers end
		size_t pos = line.find(':');
		if (pos != std::string::npos)
		{
			std::string name = line.substr(0, pos);
			std::string val = line.substr(pos + 2); // Salta el espacio después de los dos puntos.
			_map[name] = val;
		}
	}

	// Parse Content el Json enviado en el post
	std::string Content;
	while (std::getline(ss, line))
	{
		// body += line + '\n'; // Concatenate lines to form the body
		Content += line; // Concatenate lines to form the body
	}
	_map["Content"] = Content;
}

HTTPRequest::~HTTPRequest() {}

const std::map<std::string, std::string> &HTTPRequest::getMap() const { return _map; }

const std::string &HTTPRequest::getHeader(const std::string name) const { return _map.find(name)->second; }

void HTTPRequest::print()
{
	for (std::map<std::string, std::string>::iterator it = _map.begin(); it != _map.end(); it++)
		std::cout << RED << it->first << ": " << GREEN << it->second << std::endl;
	std::cout << RESET;
}
