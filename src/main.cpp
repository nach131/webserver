/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:32:24 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/12 23:08:37 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../inc/ClientParsing.hpp"
// #include "Colors.hpp"
#include "WebServer.hpp"
#include "ServerConfig.hpp"
#include "AdminServer.hpp"

void setSignals(int sig)
{
	std::cout << std::endl
			  << "ADIOSSSSSSSSSSSSSS" << std::endl;
	exit(sig);
}

void checkArg(int argc, char **argv)
{
	if (argc != 2)
		throw std::runtime_error(CYAN "Usage: ./webserv [config file]" RESET);
	else
	{
		std::string name(argv[1]);
		if (name.find(".conf") == std::string::npos ||
			name.compare(name.length() - 5, 5, ".conf") ||
			name.find(".conf", name.find(".conf") + 5) != std::string::npos)
			throw std::runtime_error(RED "File is not a configuration file!" RESET);
		std::ifstream in(argv[1]);
		if (!in.good())
			throw std::runtime_error(RED "File error!" RESET);
	}
}

int main(int argc, char **argv)
{
	try
	{
		signal(SIGINT, &setSignals);
		checkArg(argc, argv);

		// TODO
		ServerConfig config("./conf_web/mime.type");	// comprobar que exita el fichero mime.type
		config.loadConf(argv[1]);

		AdminServer server(config);
		server.run();
		// config.print();
	}
	catch (const ServerConfig::ConfErrorException &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
