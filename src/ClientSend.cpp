/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSend.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 12:28:42 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/12 13:59:39 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientSend.hpp"
#include "Colors.hpp"

// GET, POST, DELETE

ClientSend::ClientSend(const std::map<std::string, std::string> &client)
{
	for (std::map<std::string, std::string>::const_iterator it = client.begin(); it != client.end(); ++it)
	{
		std::cout << YELLOW << "Name: " << it->first << std::endl
				  << " Value: " << it->second;
		std::cout << std::endl
				  << std::endl;
	}
}
ClientSend::ClientSend(const std::string &method, const std::string &path, const std::string &prot)
{
	std::cout << "========================\n";
	std::cout << RED << "method: " << method << RESET << std::endl;
	std::cout << RED << "path: " << path << RESET << std::endl;
	std::cout << RED << "prot: " << prot << RESET << std::endl;
	std::cout << "========================\n";
}

ClientSend::~ClientSend()
{
}
