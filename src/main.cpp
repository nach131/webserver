/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:32:24 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/30 19:27:24 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../inc/ClientParsing.hpp"
#include "AdminServer.hpp"
#include "FileContent.hpp"
#include <netdb.h>
#include <fcntl.h>

int kq;
int sockfd;

void setSignals(int sig)
{
	std::cout << RESET << std::endl;
	close(kq);
	close(sockfd);
	exit(sig);
}

void checkArg(int argc, char **argv)
{
	std::ifstream in;
	std::string name, line;
	if (argc != 2)
		throw std::logic_error(CYAN "Usage: ./webserv [config file]" RESET);
	else
	{
		name = argv[1];
		if (name.find(".conf") == std::string::npos ||
			name.compare(name.length() - 5, 5, ".conf") ||
			name.find(".conf", name.find(".conf") + 5) != std::string::npos)
			throw std::logic_error(RED "Error: file is not a configuration file!" RESET);
		in.open(argv[1]);
		if (!in.good())
			throw std::runtime_error("config file error!");
	}
}

static void printServers(std::vector<ServerConfig *> &servers)
{
	for (std::vector<ServerConfig *>::iterator it = servers.begin(); it != servers.end(); it++)
		(*it)->print();
}

void createSocket()
{
	struct addrinfo *addr; // informacion sobre direcciones
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints)); // bzero to hints
	hints.ai_flags = AI_PASSIVE;	  // se especifica que debe devolver direcciones par vincular al socket de escucha
	hints.ai_family = PF_UNSPEC;	  // IPv4 o IPv6
	hints.ai_socktype = SOCK_STREAM;  // tipo del socket
	getaddrinfo("127.0.0.1", "8080", &hints, &addr);
	sockfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);

	int flags = fcntl(sockfd, F_GETFL, 0);
	if (flags == -1)
	{
		perror("fcntl");
		exit(EXIT_FAILURE);
	}
	if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		perror("fcntl");
		exit(EXIT_FAILURE);
	}

	if (bind(sockfd, addr->ai_addr, addr->ai_addrlen) < 0)
	{
		std::string errorMsg = "socket binding:\n";
		errorMsg += "Port: " + std::to_string(ntohs((reinterpret_cast<sockaddr_in *>(addr->ai_addr))->sin_port)) + "\n";
		errorMsg += strerror(errno);
		throw std::runtime_error(errorMsg);
	}

	listen(sockfd, 5);

	std::cout << "Server online.\nlistein port: " << ntohs((reinterpret_cast<sockaddr_in *>(addr->ai_addr))->sin_port) << std::endl;
	std::cout << "Servidor esperando conexiones..." << std::endl;
}

void createKqueue()
{
	kq = kqueue();
	if (kq == -1)
	{
		std::cerr << "Error creating kqueue: " << strerror(errno) << std::endl;
		throw std::runtime_error("kqueue creation failed!");
	}
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
		createKqueue();
		createSocket();

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
	catch (const std::logic_error &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (const std::runtime_error &e)
	{
		std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
		return EXIT_FAILURE;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
