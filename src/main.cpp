/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:32:24 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/20 18:51:42 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../inc/ClientParsing.hpp"
#include "AdminServer.hpp"
#include "FileContent.hpp"

void setSignals(int sig)
{
	std::cout << RESET << std::endl;
	exit(sig);
}

void checkArg(int argc, char **argv)
{
	std::ifstream in;
	std::string name, line;
	if (argc != 2)
		throw std::runtime_error(CYAN "Usage: ./webserv [config file]" RESET);
	else
	{
		name = argv[1];
		if (name.find(".conf") == std::string::npos ||
			name.compare(name.length() - 5, 5, ".conf") ||
			name.find(".conf", name.find(".conf") + 5) != std::string::npos)
			throw std::runtime_error(RED "file is not a configuration file!" RESET);
		in.open(argv[1]);
		if (!in.good())
			throw std::runtime_error(RED "file error!" RESET);
	}
	// 	for (std::vector<std::string>::iterator it = content.begin(); it != content.end(); it++)
	// 	{
	// 		std::cout << "Array line: " << *it << std::endl;
	// 	}
}

int main(int argc, char **argv)
{
	try
	{
		signal(SIGINT, &setSignals);
		std::vector<ServerConfig *> servers;
		checkArg(argc, argv);
		FileContent fc(argv[1]);
		fc.createServers(servers);
		// createServers(content, servers);
		std::cout << "Parsing ok" << std::endl;

		// TODO
		// ServerConfig config("./conf_web/mime.type");	// comprobar que exita el fichero mime.type
		// config.loadConf(argv[1]);

		// AdminServer server(config);
		// server.run();
		// config.print();
	}
	catch (const FileContent::ConfErrorException &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
