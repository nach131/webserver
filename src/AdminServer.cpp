/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AdminServer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:49:47 by nmota-bu          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/05/08 17:54:29 by nmota-bu         ###   ########.fr       */
=======
/*   Updated: 2024/05/11 13:25:41 by vduchi           ###   ########.fr       */
>>>>>>> 5fbc4ae91df505788fb1ab35c9b38a32ad4f70d8
/*                                                                            */
/* ************************************************************************** */

#include "AdminServer.hpp"

#define MAX_EVENTS 10

AdminServer::AdminServer(const ServerConfig &config) : _config(config) {}

AdminServer::~AdminServer() {}

void sendResGet(int newsockfd, const std::string &header, const std::string &content)
{
	std::cout << "[ GET ]" << std::endl;

	int n;
	// Enviar la respuesta al cliente utilizando el descriptor de archivo correcto (newsockfd)
	n = send(newsockfd, header.c_str(), header.length(), 0);
	if (n < 0)
	{
		std::cerr << "Error al enviar encabezado HTTP: " << strerror(errno) << std::endl;
		close(newsockfd);
		return; // Continuar con el siguiente intento de aceptar conexiones
	}

	// Enviar el contenido HTML utilizando el descriptor de archivo correcto (newsockfd)
	n = send(newsockfd, content.c_str(), content.length(), 0);
	if (n < 0)
	{
		std::cerr << "Error al enviar contenido HTML: " << strerror(errno) << std::endl;
		close(newsockfd);
		return; // Continuar con el siguiente intento de aceptar conexiones
	}

	std::cout << "Respuesta enviada al cliente." << std::endl;
}

void sendResPost(int newsockfd, const std::string &header, const std::string &content, char *buffer)
{
	int n;

	std::string request_to_forward = header + content;

	std::cout << ORANGE << "======[ RESPONSE POST ] ======" << std::endl;
	std::cout << request_to_forward << RESET << std::endl;

	int forward_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (forward_sockfd < 0)
	{
		std::cerr << "Error al crear socket para la conexión al otro servidor: " << strerror(errno) << std::endl;
		return;
	}

	struct sockaddr_in forward_serverAddr;
	memset(&forward_serverAddr, 0, sizeof(forward_serverAddr));
	forward_serverAddr.sin_family = AF_INET;
	forward_serverAddr.sin_port = htons(3000);
	// forward_serverAddr.sin_addr.s_addr = inet_addr("192.168.39.140");
	forward_serverAddr.sin_addr.s_addr = inet_addr("192.168.1.20");

	if (connect(forward_sockfd, (struct sockaddr *)&forward_serverAddr, sizeof(forward_serverAddr)) < 0)
	{
		std::cerr << "Error al conectar al otro servidor: " << strerror(errno) << std::endl;
		close(forward_sockfd);
		return;
	}

	n = send(forward_sockfd, request_to_forward.c_str(), request_to_forward.size(), 0);
	if (n < 0)
	{
		std::cerr << "Error al enviar datos al otro servidor: " << strerror(errno) << std::endl;
		close(forward_sockfd);
		return;
	}

	std::cout << "Solicitud enviada al otro servidor." << std::endl;

	std::string response_from_server;
	while ((n = recv(forward_sockfd, buffer, sizeof(buffer), 0)) > 0)
	{
		response_from_server.append(buffer, n);
	}

	if (n < 0)
	{
		std::cerr << "Error al recibir datos del otro servidor: " << strerror(errno) << std::endl;
		close(forward_sockfd);
		return;
	}

	std::cout << "Respuesta del otro servidor recibida." << std::endl;

	send(newsockfd, response_from_server.c_str(), response_from_server.size(), 0);

	std::cout << "Respuesta enviada al cliente." << std::endl;

	close(forward_sockfd);
}

void uploadFile(int newsockfd)
{
	int n;
	std::string response = "HTTP/1.1 200 OK\r\n"
						   "Content-Type: text/html\r\n"
						   "\r\n"
						   "<!DOCTYPE html>\n"
						   "<html>\n"
						   "<head>\n"
						   "<title>Archivo Subido</title>\n"
						   "</head>\n"
						   "<body>\n"
						   "<h1>Archivo Subido Exitosamente</h1>\n"
						   "</body>\n"
						   "</html>\n";

	// std::string request_to_forward = header + response;

	n = send(newsockfd, response.c_str(), response.size(), 0);
	if (n < 0)
	{
		std::cerr << "Error al enviar datos al otro servidor: " << strerror(errno) << std::endl;
		return;
	}
}

//===============PRINT====================================================
void printEvent(const struct kevent &event)
{
	std::cout << GREEN;
	std::cout << "Identificador: " << event.ident << std::endl;
	std::cout << "Filtro: " << event.filter << std::endl;
	std::cout << "Flags: " << event.flags << std::endl;
	std::cout << "Filtros específicos: " << event.fflags << std::endl;
	std::cout << "Datos: " << event.data << std::endl;
	std::cout << "Datos de usuario: " << event.udata << std::endl;
	std::cout << RESET;
}

void printPeticion(const char *buffer)
{
	std::cout << CYAN "[ Mensaje del cliente: ]\n"
			  << buffer << RESET << std::endl;
}

void printResponse(std::string header, std::string content)
{
	(void)content;
	std::cout << YELLOW << "======[ RESPONSE ] ======" << std::endl;
	std::cout << "[ HEADER ]" << std::endl;
	std::cout << header << std::endl;
	// std::cout << "[ CONTENT ]" << std::endl;
	// std::cout << content << std::endl;
	std::cout << "========================" << RESET << std::endl;
}

//=========================================================================

void AdminServer::run()
{
	// int sockfd, newsockfd;
	// socklen_t client;
	// char buffer[1024];
	// struct sockaddr_in serverAddr, clientAddr;
	// int n;

	int newsockfd;
	socklen_t client;
	// char buffer[1024];
	struct sockaddr_in clientAddr;
	int n;

	/*
	// Configurar la dirección del servidor
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8080);				// Puerto del servidor
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Escuchar en todas las interfaces de red

	// Crear socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// sockfd = socket(AF_MAX, SOCK_STREAM, 0); // Error socket
	if (sockfd < 0)
	{
		std::string errorMsg = RED "Error creating socket:\n";
		errorMsg += CYAN;
		errorMsg += strerror(errno);
		throw std::runtime_error(errorMsg);
	}

	// Enlazar el socket a la dirección del servidor
	if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		std::string errorMsg = RED "Socket binding error:\n";
		errorMsg += CYAN + std::to_string(ntohs(serverAddr.sin_port)) + " ";
		errorMsg += strerror(errno);
		throw std::runtime_error(errorMsg);
	}

	// Escuchar por conexiones entrantes
	listen(sockfd, 5);
	*/

	std::cout << "sockfd: " << sockfd << std::endl;
	std::cout << "Servidor esperando conexiones..." << std::endl;	

	//=========================================================================

	// Crear kqueue
	int kq = kqueue();
	if (kq == 0)
	{
		std::cerr << "Error creating kqueue: " << strerror(errno) << std::endl;
		close(_config.getServerSocket());
		return;
	}

	// Configurar evento para el socket de escucha
<<<<<<< HEAD
	struct kevent eventRead;
	EV_SET(&eventRead, sockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
=======
	struct kevent change;
	EV_SET(&change, _config.getServerSocket(), EVFILT_READ, EV_ADD, 0, 0, NULL);
>>>>>>> 5fbc4ae91df505788fb1ab35c9b38a32ad4f70d8

	// Registro de sockfd para eventos de lectura
	if (kevent(kq, &eventRead, 1, NULL, 0, NULL) == -1)
	{
		std::cerr << "Error en kevent: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	// Configurar evento para el socket de escucha para escritura
	struct kevent eventWrite;
	EV_SET(&eventWrite, sockfd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);

	// Registro de sockfd para eventos de escritura
	if (kevent(kq, &eventWrite, 1, NULL, 0, NULL) == -1)
	{
		std::cerr << "Error en kevent para escritura: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	// Array para eventos
	struct kevent events[MAX_EVENTS];

	//=========================================================================

	// Inicializar el conjunto de descriptores de archivo, para CGI
	// fd_set fdSet;
	// FD_ZERO(&fdSet);

	//=========================================================================

	while (42)
	{

		// Esperar eventos en kqueue
		int nev = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL); // miarar kqueue.cpp para timeout
 		if (nev < 0)
		{
			std::cerr << "Error en kevent: " << strerror(errno) << std::endl;
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < nev; ++i)
		{
<<<<<<< HEAD
			std::cout << "i: "<< i << ", nev: " << nev << std::endl;
			

if (events[i].ident == (unsigned long)sockfd)
{
    // Verifica si el evento es para lectura o escritura

		std::cout << CYAN << "toma: " << events[i].filter << std::endl;

    if (events[i].filter == EVFILT_READ)
    {
        std::cout << CYAN << "EVENTO lectura" << RESET << std::endl;
    }
    else if (events[i].filter == EVFILT_WRITE)
    {
        std::cout << CYAN << "EVENTO escritura" << RESET << std::endl;
    }
}



			if (events[i].ident == (unsigned long)sockfd)
=======
			if (events[i].ident == (unsigned long)_config.getServerSocket())
>>>>>>> 5fbc4ae91df505788fb1ab35c9b38a32ad4f70d8
			{
				// Aceptar la conexión entrante
				newsockfd = accept(_config.getServerSocket(), (struct sockaddr *)&clientAddr, &client);
				// newsockfd = -1; // error
				if (newsockfd < 0)
				{
					std::cerr << "Error al aceptar conexión: " << strerror(errno) << std::endl;
					continue; // Continuar con el siguiente intento de aceptar conexiones
				}

				std::cout << MAGENTA << "Conexión aceptada. Socket del cliente: " << newsockfd << RESET << std::endl;

				// Registro de newsockfd para eventos de lectura
				EV_SET(&events[i], newsockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);

				// TODO
				std::cout << "[ conexion entrante ]" << std::endl;
				std::cout << "events: " << i << std::endl;
				printEvent(events[i]);

				if (kevent(kq, &events[i], 1, NULL, 0, NULL) == -1)
				{
					std::cerr << "Error en kevent: " << strerror(errno) << std::endl;
					exit(EXIT_FAILURE);
				}
				else
				{
					// Recibir datos del cliente
					// memset(buffer, 0, sizeof(buffer));
					n = recv(newsockfd, _config.getBuffer(), 1024, 0);
					if (n < 0)
					{
						std::cerr << "Error al recibir datos: " << strerror(errno) << std::endl;
						close(newsockfd);
						continue; // Continuar con el siguiente intento de aceptar conexiones
					}

					//===================PETICION==============================================
					// TODO
					printPeticion(_config.getBuffer());
					//===================PARSING==============================================

					HTTPRequest request(_config.getBuffer());
					// TODO
					request.print();

					//=========================================================================

					// HTTPBody body(request);
					HTTPRes response(request, &_config);
					// TODO
					printResponse(response.getHeader(), response.getContent());

					//=========================================================================

					// Enviar la respuesta al cliente utilizando la función creada
					if (request.getHeader("Method") == "GET")
					{
						sendResGet(newsockfd, response.getHeader(), response.getContent());
					}
					else if (request.getHeader("Method") == "POST")
					{
						std::cout << "[ POST ]" << std::endl;
						if (request.getHeader("Path") == "/submit")
						{
							std::cout << "FILE" << std::endl;
							uploadFile(newsockfd);
						}
						else
							sendResPost(newsockfd, response.getHeader().c_str(), response.getContent(), _config.getBuffer());
					}

					else if (request.getHeader("Method") == "DELETE")
					{
						std::cout << "ES DELETE\n";
					}

					//=========================================================================
					// Cerrar el socket de la conexión actual
					// close(events[i].ident);
					close(newsockfd);

					// TODO
					// [ Server Configuration ]
					_config.print();
				}
			}
		}
	}

	// Cerrar el socket del servidor (esto no se alcanzará)
	close(_config.getServerSocket());
}
