/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 11:58:24 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/22 00:29:55 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPBody.hpp"
#include "HTTPRequest.hpp"
#include "HTTPHeader.hpp"
#include "WebServer.hpp"

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

	// TODO
	// favicon.ico si no hay, enviar respuesta generica

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

		HTTPRequest request(buffer);

		// HTTPRequest httpRequest(buffer);

		std::cout << "=========map request===============\n";
		request.print();
		std::cout << "========================\n";
		std::cout << "Method: " << request.getHeader("Method") << std::endl;
		std::cout << "Path: " << request.getHeader("Path") << std::endl;
		std::cout << "Version: " << request.getHeader("Version") << std::endl;
		std::cout << "Content-Type: " << request.getHeader("Content-Type") << std::endl;
		std::cout << "Accept-Language: " << request.getHeader("Accept-Language") << std::endl;
		std::cout << "Content-Length: " << request.getHeader("Content-Length") << std::endl;
		std::cout << "Body: " << request.getHeader("Body") << std::endl;
		std::cout << "========================\n";

		//=========================================================================

		// ClientSend clientConMap(pars.getMap());
		HTTPBody body(request);

		//======================STRING=============================================
		HTTPHeader header(request.getProt(), body.code());

		// TODO
		// Content-Type: text/html; charset=UTF-8
		// falta indicar el tipo de charset
		//
		// header.addField("Content-Type", "text/html; charset=UTF-8");
		header.addField("Content-Type", body.mime());
		header.addField("Content-Length", std::to_string(body.content().length()));
		header.addField("Date", DateField());
		header.addField("Accept-Language", request.getHeader("Accept-Language"));

		// Obtener el encabezado HTTP como una cadena y mostrarlo
		std::string heaStr = header.getHeader();
		std::cout << GREEN << "[ Header ]\n"
				  << heaStr << RESET << std::endl;
		std::cout << heaStr.length() << std::endl;
		std::cout << "========================\n";

		n = write(newsockfd, header.getHeader().c_str(), header.getHeader().length());

		if (n < 0)
		{
			std::cerr << "Error al enviar encabezado HTTP: " << strerror(errno) << std::endl;
			close(newsockfd);
			continue; // Continuar con el siguiente intento de aceptar conexiones
		}

		// Enviar contenido HTML
		n = write(newsockfd, body.content().c_str(), body.content().length());
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
