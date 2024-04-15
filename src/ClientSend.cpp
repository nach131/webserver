/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSend.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 12:28:42 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/15 20:04:43 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientSend.hpp"
#include "Colors.hpp"

// GET, POST, DELETE

int ClientSend::methodGet(ClientSend &client, std::string path, std::string prot)
{
	std::cout << "==========GET===========\n";
	// std::cout << RED << "method: " << method << RESET << std::endl;
	std::cout << RED << "path: " << path << RESET << std::endl;
	std::cout << RED << "prot: " << prot << RESET << std::endl;
	std::cout << "========================\n";

	std::ifstream file;
	if (path == "/")
		file.open("./dist/index.html");
	else
	{
		std::string filePath = "./dist" + path;
		file.open(filePath.c_str());
	}

	if (!file)
	{
		std::cerr << "Error al abrir el archivo " << path << std::endl;
		// exit(EXIT_FAILURE);
	}
	else
	{
		std::ostringstream oss;
		oss << file.rdbuf();		 // Leer el contenido del archivo en un stringstream
		client._content = oss.str(); // Obtener el contenido como string
	}

	return 0;
}

int ClientSend::methodPost(ClientSend &client, std::string path, std::string prot)
{
	(void)client;
	std::cout << "==========POST==========\n";
	// std::cout << RED << "method: " << method << RESET << std::endl;
	std::cout << RED << "path: " << path << RESET << std::endl;
	std::cout << RED << "prot: " << prot << RESET << std::endl;
	std::cout << "========================\n";

	return 0;
}

int ClientSend::methodDelete(ClientSend &client, std::string path, std::string prot)
{
	(void)client;
	std::cout << "=========DELETE==========\n";
	// std::cout << RED << "method: " << method << RESET << std::endl;
	std::cout << RED << "path: " << path << RESET << std::endl;
	std::cout << RED << "prot: " << prot << RESET << std::endl;
	std::cout << "========================\n";
	return 0;
}

// // POR SI USAMOS SOLO MAP
// ClientSend::ClientSend(const std::map<std::string, std::string> &client)
// {
// 	for (std::map<std::string, std::string>::const_iterator it = client.begin(); it != client.end(); ++it)
// 	{
// 		std::cout << YELLOW << "Name: " << it->first << std::endl
// 				  << " Value: " << it->second << std::endl;
// 	}
// }

ClientSend::ClientSend(const ClientParsing & pars)
{
	std::map<std::string, int (*)(ClientSend &client, std::string, std::string)> map;

	map["GET"] = &methodGet;
	map["POST"] = &methodPost;
	map["DELETE"] = &methodDelete;

	std::map<std::string, int (*)(ClientSend &client, std::string, std::string)>::iterator it = map.find(pars.getMethod());

	if (it != map.end())
		it->second(*this, pars.getPath(), pars.getProt());
	else
		std::cout << "NotFoud: " << pars.getMethod() << std::endl;
}

ClientSend::~ClientSend() {}

std::string const &ClientSend::content() const { return _content; }
