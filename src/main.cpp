/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:32:24 by vduchi            #+#    #+#             */
/*   Updated: 2024/04/29 15:28:44 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../inc/ClientParsing.hpp"
// #include "Colors.hpp"
# include "WebServer.hpp"
# include "ServerConfig.hpp"


void checkArg(int argc, char **argv)
{
	if (argc != 2)
		throw std::runtime_error(CYAN "Usage: ./webserv [config file]" RESET);
	else
	{
		std::string name(argv[1]);
		if (name.compare(name.length() - 4, 4, ".cnf") || name.find(".cnf", name.find(".cnf") + 4) != std::string::npos)
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
		(void)argc;
		(void)argv;
		//		checkArg(argc, argv);

		ServerConfig config;
		config.loadConf("argv[1]");
		config.print();	


		start();
		std::cout << "start\n";
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
