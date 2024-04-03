/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:32:24 by vduchi            #+#    #+#             */
/*   Updated: 2024/04/03 11:54:22 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/WebServer.hpp"

static int outError(int idx)
{
	std::cout << RED;
	idx == 1 ? std::cout << "Too many arguments!" << std::endl : std::cout << "";
	idx == 2 ? std::cout << "File is not a configuration file!" << std::endl : std::cout << "";
	idx == 3 ? std::cout << "File error!" << std::endl : std::cout << "";
	std::cout << CYAN;
	std::cout << "Usage: ./webserv [config file]" << RESET << std::endl;
	return EXIT_FAILURE;
}

static int checkFile(char *arg)
{
	std::string name(arg);
	if (name.compare(name.length() - 4, 4, ".cnf") || name.find(".cnf", name.find(".cnf") + 4) != std::string::npos)
		return outError(2);
	std::ifstream in(arg);
	if (!in.good())
		return outError(3);
	return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
	switch (argc)
	{
	case 1:
		return outError(0);
	case 2:
		return checkFile(argv[1]);
	default:
		return outError(1);
	}
}
