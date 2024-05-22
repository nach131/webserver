/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:32:24 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/22 11:28:48 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../inc/ClientParsing.hpp"
#include "AdminServer.hpp"
#include "FileContent.hpp"
#include <netdb.h>

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

static void printServers(std::vector<ServerConfig *> &servers)
{
	for (std::vector<ServerConfig *>::iterator it = servers.begin(); it != servers.end(); it++)
		(*it)->print();
}

int createSocket()
{
	struct addrinfo *addr; // informacion sobre direcciones
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints)); // bzero to hints
	hints.ai_flags = AI_PASSIVE;	  // se especifica que debe devolver direcciones par vincular al socket de escucha
	hints.ai_family = PF_UNSPEC;	  // IPv4 o IPv6
	hints.ai_socktype = SOCK_STREAM;  // tipo del socket
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
		signal(SIGINT, &setSignals);
		// checkArg(argc, argv);
		(void)argv;
		(void)argc;
		std::vector<ServerConfig *> servers;
		// FileContent fc(argv[1]);
		FileContent fc("./conf_web/default.conf");
		fc.createServers(servers);
		printServers(servers);
		// createServers(content, servers);
		std::cout << "Parsing ok" << std::endl;

		// TODO
		AdminServer server(*servers[0]);
		int sockfd = createSocket();
		int kq = createKqueue();

		// // Configurar evento para el socket de escucha
		struct kevent eventSet;
		EV_SET(&eventSet, sockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
		kevent(kq, &eventSet, 1, NULL, 0, NULL);

		server.run(sockfd, kq);
		// config.print();
	}
	catch (const FileContent::ConfErrorException &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (const KeyValue::KeyValueException &e)
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
