/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:32:24 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/17 23:41:11 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../inc/ClientParsing.hpp"
// #include "Colors.hpp"
#include "WebServer.hpp"
#include "ServerConfig.hpp"
#include "AdminServer.hpp"
#include <netdb.h>

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

int createSocket()
{
	struct addrinfo *addr; // informacion sobre direcciones
	struct addrinfo hints;
	memset(&hints, 0, sizeof hints); // bzero to hints
	hints.ai_flags = AI_PASSIVE;	 // se especifica que debe devolver direcciones par vincular al socket de escucha
	hints.ai_family = PF_UNSPEC;	 // IPv4 o IPv6
	hints.ai_socktype = SOCK_STREAM; // tipo del socket
	getaddrinfo("127.0.0.1", "8080", &hints, &addr);
	int sockfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);

	if (bind(sockfd, addr->ai_addr, addr->ai_addrlen) < 0)
	{
		std::string errorMsg = RED "Socket binding error:\n";
		errorMsg += "Puerto: " + std::to_string(ntohs((reinterpret_cast<sockaddr_in *>(addr->ai_addr))->sin_port)) + "\n";
		errorMsg += strerror(errno);
		throw std::runtime_error(errorMsg);
	}

	listen(sockfd, 5);

	std::cout << "Server online.\nlistein port: " << ntohs((reinterpret_cast<sockaddr_in *>(addr->ai_addr))->sin_port) << std::endl;
	std::cout << "Servidor esperando conexiones..." << std::endl;

	return sockfd;
}

int createKqueue()
{
	int kq = kqueue();
	if (kq == -1)
	{
		std::cerr << "Error creating kqueue: " << strerror(errno) << std::endl;
		throw std::runtime_error("Error creating kqueue");
	}
	return kq;
}

int main(int argc, char **argv)
{
	try
	{
		(void)argc;
		(void)argv;
		//		checkArg(argc, argv);

		// TODO
		ServerConfig config("./conf_web/mime.type"); // comprobar que exita el fichero mime.type
		config.loadConf("argv[1]");
		AdminServer server(config);
		int sockfd = createSocket();
		int kq = createKqueue();

		// Configurar evento para el socket de escucha
		struct kevent eventSet;
		EV_SET(&eventSet, sockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
		kevent(kq, &eventSet, 1, NULL, 0, NULL);

		server.run(sockfd, kq);
		// config.print();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
