/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientParsing.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:05:47 by vduchi            #+#    #+#             */
/*   Updated: 2024/04/12 12:51:38 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ClientParsing.hpp"

ClientParsing::ClientParsing(const char *buf)
{
	int i = 0;
	std::string first, el, line, input(buf);
	std::stringstream ss(input);
	std::getline(ss, first);
	std::stringstream ssFirst(first);

	while (std::getline(ssFirst, el, ' '))
	{
		switch (i) {
			case 0:
				_method = el;
				break;
			case 1:
				_path = el;
				break;
			case 2:
				_prot = el;
				break;
		}
		i++;
	}

	while (std::getline(ss, line))
	{
		if (line.length() == 1)
			continue;
		std::cout << "Line: " << line << std::endl;
		std::string name, val;
		std::stringstream ssEl(line);
		std::getline(ssEl, name, ' ');
		std::getline(ssEl, val);
		_map[name] = val;
	}

	std::cout << std::endl;

	// for (std::map<std::string, std::string>::iterator it = _map.begin(); it != _map.end(); it++)
	// {
	// 	std::cout << "Name: " << it->first << std::endl <<  " Value: " << it ->second;
	// 	std::cout << std::endl << std::endl;
	// }
}

ClientParsing::~ClientParsing() {}

const std::string ClientParsing::getPath() const { return _path; }

const std::string ClientParsing::getProt() const { return _prot; }

const std::string ClientParsing::getMethod() const { return _method; }

const std::map<std::string, std::string> &ClientParsing::getMap() const { return _map; }
