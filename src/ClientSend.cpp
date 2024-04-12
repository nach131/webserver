/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSend.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 12:28:42 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/12 17:55:40 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientSend.hpp"
#include "Colors.hpp"

// GET, POST, DELETE

int mGet(std::string path, std::string prot)
{
	std::cout << "==========GET===========\n";
	// std::cout << RED << "method: " << method << RESET << std::endl;
	std::cout << RED << "path: " << path << RESET << std::endl;
	std::cout << RED << "prot: " << prot << RESET << std::endl;
	std::cout << "========================\n";
	return 0;
}

int mPost(std::string path, std::string prot)
{
	std::cout << "==========POST==========\n";
	// std::cout << RED << "method: " << method << RESET << std::endl;
	std::cout << RED << "path: " << path << RESET << std::endl;
	std::cout << RED << "prot: " << prot << RESET << std::endl;
	std::cout << "========================\n";
	return 0;
}

int mDelete(std::string path, std::string prot)
{
	std::cout << "=========DELETE==========\n";
	// std::cout << RED << "method: " << method << RESET << std::endl;
	std::cout << RED << "path: " << path << RESET << std::endl;
	std::cout << RED << "prot: " << prot << RESET << std::endl;
	std::cout << "========================\n";
	return 0;
}

ClientSend::ClientSend(const std::map<std::string, std::string> &client)
{
	for (std::map<std::string, std::string>::const_iterator it = client.begin(); it != client.end(); ++it)
	{
		std::cout << YELLOW << "Name: " << it->first << std::endl
				  << " Value: " << it->second << std::endl;
	}
}
ClientSend::ClientSend(const std::string &method, const std::string &path, const std::string &prot)
{

	std::map<std::string, int (*)(std::string, std::string)> map;

	map["GET"] = &mGet;
	map["POST"] = &mPost;
	map["DELETE"] = &mDelete;

	std::map<std::string, int (*)(std::string, std::string)>::iterator it = map.find(method);

	if (it != map.end())
	{
		it->second(path, prot);
	}
	else
	{
		std::cout << "NotFoud: " << method << std::endl;
	}
}

ClientSend::~ClientSend() {}
