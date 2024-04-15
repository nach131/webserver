/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 11:58:24 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/15 11:46:21 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ClientParsing.hpp"
#include "ClientSend.hpp"

// Cabeceras específicas de sockets en sistemas tipo Unix
// #include <sys/types.h>
// #include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int start()
{
	int sockfd, newsockfd;
	socklen_t clilen;
	char buffer[1024];
	struct sockaddr_in serverAddr, clientAddr;
	int n;

	std::string htmlContent =
		"<!DOCTYPE html>\n"
		"<html>\n"
		"<head>\n"
		"<link rel=\"stylesheet\" href=\"mystyle.css\">\n"
		"</head>\n"
		"<body>\n"
		"\n"
		"<h1>Toma mi Heading</h1>\n"
		"<p>Toma mas tomate.</p>\n"
		"\n"
		"</body>\n"
		"</html>\n";

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

	// Configurar la dirección del servidor
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8080);				// Puerto del servidor
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Escuchar en todas las interfaces de red

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

	std::cout << "Servidor esperando conexiones..." << std::endl;

	while (42)
	{
		clilen = sizeof(clientAddr);

		// Aceptar la conexión entrante
		newsockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clilen);

		// newsockfd = -1; // error
		// (void)clilen;	// error
		if (newsockfd < 0)
		{
			std::cerr << "Error al aceptar conexión: " << strerror(errno) << std::endl;
			continue; // Continuar con el siguiente intento de aceptar conexiones
		}

		std::cout << MAGENTA << "Conexión aceptada. Socket del cliente: " << newsockfd << RESET << std::endl;

		// Recibir datos del cliente
		memset(buffer, 0, sizeof(buffer));
		n = recv(newsockfd, buffer, sizeof(buffer), 0);
		if (n < 0)
		{
			std::cerr << "Error al recibir datos: " << strerror(errno) << std::endl;
			close(newsockfd);
			continue; // Continuar con el siguiente intento de aceptar conexiones
		}

		std::cout << CYAN "Mensaje del cliente: \n"
				  << buffer << RESET << std::endl;

		//===================PARSING==============================================

		ClientParsing pars(buffer);
		// std::cout << GREEN << "Method: " << pars.getMethod() << " Path: " << pars.getPath();
		// std::cout << " Protocol: " << pars.getProt() << RESET << std::endl;

		//=========================================================================

		// ClientSend clientConMap(pars.getMap());
		ClientSend client(pars.getMethod(), pars.getPath(), pars.getProt());

		//=========================================================================
		std::string header = "HTTP/1.1 200 OK\r\n";
		header += "Content-Type: text/html\r\n";
		// header += "Content-Length: " + std::to_string(htmlContent.length()) + "\r\n\r\n";
		header += "Content-Length: " + std::to_string(client.content().length()) + "\r\n\r\n";

		n = write(newsockfd, header.c_str(), header.length());
		if (n < 0)
		{
			std::cerr << "Error al enviar encabezado HTTP: " << strerror(errno) << std::endl;
			close(newsockfd);
			continue; // Continuar con el siguiente intento de aceptar conexiones
		}

		// Enviar contenido HTML
		n = write(newsockfd, client.content().c_str(), client.content().length());
		if (n < 0)
		{
			std::cerr << "Error al enviar contenido HTML: " << strerror(errno) << std::endl;
			close(newsockfd);
			continue; // Continuar con el siguiente intento de aceptar conexiones
		}
		//=========================================================================
		// Cerrar el socket de la conexión actual
		close(newsockfd);

		std::cout << "Respuesta enviada al cliente." << std::endl;
	}

	// Cerrar el socket del servidor (esto no se alcanzará)
	close(sockfd);
	return 0;
}
