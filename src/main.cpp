/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:32:24 by vduchi            #+#    #+#             */
/*   Updated: 2024/04/03 19:52:16 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

void check_arg(int argc, char **argv)
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
		check_arg(argc, argv);
		// start();
		std::cout << "start\n";
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
