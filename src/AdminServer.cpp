/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AdminServer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:49:47 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/02 17:42:45 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AdminServer.hpp"

AdminServer::AdminServer(const ServerConfig &config) : _config(config) {}

AdminServer::~AdminServer() {}

void sendResGet(int newsockfd, const std::string &header, const std::string &content)
{
	std::cout << "[ GET ]" << std::endl;

	int n;

	// Enviar el encabezado HTTP
	n = write(newsockfd, header.c_str(), header.length());
	if (n < 0)
	{
		std::cerr << "Error al enviar encabezado HTTP: " << strerror(errno) << std::endl;
		close(newsockfd);
		return; // Salir de la función si hay un error
	}

	// Enviar el contenido HTML
	n = write(newsockfd, content.c_str(), content.length());
	if (n < 0)
	{
		std::cerr << "Error al enviar contenido HTML: " << strerror(errno) << std::endl;
		close(newsockfd);
		return; // Salir de la función si hay un error
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

void AdminServer::run()
{
	int sockfd, newsockfd;
	socklen_t client;
	struct sockaddr_in serverAddr, clientAddr;
	int n;

	char buffer[1024];

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

	// Configurar pollfd para el socket de escucha
	struct pollfd fds[1];
	fds[0].fd = sockfd;
	fds[0].events = POLLIN;

	int timeout = 1; // milisegundos (1 segundos)
	time_t last_activity = time(NULL); // Guardar el tiempo de la última actividad

	int cuenta = 0;
	bool toma = false;

	while (42)
	{

		 // Calcular el tiempo de inactividad
        time_t current_time = time(NULL); // tiempo actual en segundos
        time_t inactive_time = current_time - last_activity;


		// Si no hay actividad durante el tiempo de espera, cerrar la conexión
        if (inactive_time >= timeout)
        {
            std::cout << "Tiempo de inactividad 2 segundos" << std::endl;
			cuenta = 0;
			toma = false;
			last_activity = time(NULL);
        }

std::cout << "1 valor toma: " << toma<<std::endl;
		std::cout << "last_activity: " << last_activity << std::endl;
		std::cout << "current_time: " << current_time << std::endl;
		std::cout << "inactive_time: " << inactive_time << std::endl;

//=========================================================================
		
		// Esperar eventos en el socket de escucha
		if (poll(fds, 1, -1) < 0)
		{
			std::cerr << "Error en poll(): " << strerror(errno) << std::endl;
			continue;
		}

		if (fds[0].revents & POLLIN)
		{
			cuenta++;
			client = sizeof(clientAddr);

			// Aceptar la conexión entrante
			newsockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &client);

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

			//===================PETICION==============================================
			std::cout << CYAN "[ Mensaje del cliente: ]\n"
					  << buffer << RESET << std::endl;

			//===================PARSING==============================================

			HTTPRequest request(buffer);

			request.print();

			//=========================================================================

			// HTTPBody body(request);
			HTTPRes response(request, _config);


			std::cout << YELLOW << "======[ RESPONSE ] ======" << std::endl;
			std::cout << "[ HEADER ]" << std::endl;
			std::cout << response.getHeader() << std::endl;
			// std::cout << "[ CONTENT ]" << std::endl;
			// std::cout << response.getContent() << std::endl;
			std::cout << "========================" << RESET << std::endl;

			//=========================================================================

			// Enviar la respuesta al cliente utilizando la función creada
			if (request.getHeader("Method") == "GET")
			{
				// AQUI PARA LOCATIONS ?


				sendResGet(newsockfd, response.getHeader().c_str(), response.getContent());
			}
			else if (request.getHeader("Method") == "POST")
			{
				std::cout << "[ POST ]" << std::endl;
				if (request.getHeader("Path") == "/submit")
				// TODO
				// DESPUES HACER REDIRECCCIONAMIENRTO A LA URL DE DONDE VENIA
				{
					std::cout << "FILE" << std::endl;
					uploadFile(newsockfd);
				}
				else
					sendResPost(newsockfd, response.getHeader().c_str(), response.getContent(), buffer);
			}

			else if (request.getHeader("Method") == "DELETE")
			{
				std::cout << "ES DELETE\n";
			}

			//=========================================================================
			// Cerrar el socket de la conexión actual
			close(newsockfd);
		}

std::cout << "cuenta: " << cuenta << std::endl;
std::cout << "2 valor toma: " << toma<<std::endl;

	
	}

	// Cerrar el socket del servidor (esto no se alcanzará)
	close(sockfd);
}
