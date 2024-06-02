/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:32:24 by vduchi            #+#    #+#             */
/*   Updated: 2024/06/02 13:07:13 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AdminServer.hpp"
#include "FileContent.hpp"
#include <netdb.h>
#include <fcntl.h>

int kq;

void setSignals(int sig)
{
	if (sig == SIGINT)
	{
		close(kq);
		exit(sig);
	}
}

void checkArg(int argc, char **argv)
{
	std::ifstream in;
	std::string name, line;
	if (argc == 1)
		return;
	if (argc > 2)
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

// static void printServers(std::vector<ServerConfig *> &servers)
// {
// 	for (std::vector<ServerConfig *>::iterator it = servers.begin(); it != servers.end(); it++)
// 		(*it)->print();
// }

void createSocket(std::vector<ServerConfig *> servers)
{
	int i = 0;
	int *data;
	struct kevent eventSet[MAX_EVENTS];
	for (std::vector<ServerConfig *>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		struct addrinfo *addr;
		struct addrinfo hints;
		memset(&hints, 0, sizeof(hints));
		hints.ai_flags = AI_PASSIVE;
		hints.ai_family = PF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		getaddrinfo("127.0.0.1", intToString((*it)->getPort()).c_str(), &hints, &addr);
		(*it)->setServerSocket(socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol));

		int flags = fcntl((*it)->getServerSocket(), F_GETFL, 0);
		if (flags == -1)
		{
			std::string err(strerror(errno));
			throw std::runtime_error("fcntl flags creation failed: " + err);
		}
		if (fcntl((*it)->getServerSocket(), F_SETFL, flags | O_NONBLOCK) == -1)
		{
			std::string err(strerror(errno));
			throw std::runtime_error("fcntl flags creation failed: " + err);
		}

		if (bind((*it)->getServerSocket(), addr->ai_addr, addr->ai_addrlen) < 0)
		{
			std::string errorMsg = "socket binding\n";
			errorMsg += "Port: " + std::to_string(ntohs((reinterpret_cast<sockaddr_in *>(addr->ai_addr))->sin_port)) + "\n";
			errorMsg += strerror(errno);
			throw std::runtime_error(errorMsg);
		}
		listen((*it)->getServerSocket(), 5);

		data = (int *)calloc(1, sizeof(int));
		data[0] = i;
		(*it)->setData(data);
		EV_SET(&eventSet[i], (*it)->getServerSocket(), EVFILT_READ, EV_ADD, 0, 0, data);
		kevent(kq, &eventSet[i], 1, NULL, 0, NULL);
		std::cout << CYAN "Server online -> listening port: " << ntohs((reinterpret_cast<sockaddr_in *>(addr->ai_addr))->sin_port) << RESET << std::endl;
		// std::cout << "Servidor " << i << " esperando conexiones..." << std::endl;
		i++;
	}
}

void createKqueue()
{
	kq = kqueue();
	if (kq == -1)
	{
		std::string err("creating kqueue: ");
		err += strerror(errno);
		err += "\n";
		throw std::runtime_error(err);
	}
}

int main(int argc, char **argv)
{
	try
	{
		signal(SIGINT, &setSignals);
		signal(SIGQUIT, &setSignals);
		signal(SIGTSTP, &setSignals);
		checkArg(argc, argv);
		std::vector<ServerConfig *> servers;
		FileContent fc;
		if (argv[1])
			fc.openFile(argv[1]);
		else
			fc.openFile("./conf_web/default.conf");
		fc.createServers(servers);
		AdminServer server(servers);
		createKqueue();
		createSocket(servers);
		server.run(kq);
	}
	catch (const FileContent::ConfErrorException &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (const ServerConfig::ConfErrorException &e)
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
