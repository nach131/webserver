/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AdminServer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:49:47 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/13 17:13:34 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AdminServer.hpp"

// #define NUM_CLIENTS 10
#define MAX_EVENTS 32
#define MAX_MSG_SIZE 8192

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

void printPeticion(const std::string buffer)
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

struct client_data
{
	int fd;
	std::string path;
	std::string prePath;
} clients[MAX_EVENTS];

int getConnect(int fd)
{
	for (int i = 0; i < NUM_CLIENTS; i++)
		if (clients[i].fd == fd)
			return i;
	return -1;
}

int addConnect(int fd)
{
	if (fd < 1)
		return -1;
	int i = getConnect(0);
	if (i == -1)
		return -1;
	clients[i].fd = fd;
	return 0;
}

int delConnect(int fd)
{
	if (fd < 1)
		return -1;
	int i = getConnect(fd);
	if (i == -1)
		return -1;
	clients[i].fd = 0;
	return close(fd);
}

void AdminServer::run(int sockfd, int kq)
{

	char buffer[MAX_MSG_SIZE];
	struct kevent evSet;
	struct kevent evList[MAX_EVENTS];
	struct sockaddr_storage addr;
	socklen_t socklen = sizeof(addr);

	while (42)
	{
		int num_events = kevent(kq, NULL, 0, evList, MAX_EVENTS, NULL);
		for (int i = 0; i < num_events; i++)
		{
			// receive new connection
			if (evList[i].ident == (unsigned long)sockfd)
			{
				int fd = accept(evList[i].ident, (struct sockaddr *)&addr, &socklen);
				if (addConnect(fd) == 0)
				{
					// EV_SET(&evSet, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
					// EV_FLAG0 PAR LA PRIMERA PETICION
					EV_SET(&evSet, fd, EVFILT_READ, EV_ADD | EV_FLAG0, 0, 0, NULL);
					kevent(kq, &evSet, 1, NULL, 0, NULL);
					// send_welcome_msg(fd);
				}
				else
				{
					std::cout << "connection refused." << std::endl;
					close(fd);
				}
			} // client disconnected
			else if (evList[i].flags & EV_EOF)
			{
				int fd = evList[i].ident;
				std::cout << "client #" << getConnect(fd) << " disconnected." << std::endl;
				EV_SET(&evSet, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
				kevent(kq, &evSet, 1, NULL, 0, NULL);
				delConnect(fd);
			} // read message from client
			else if (evList[i].filter == EVFILT_READ)
			{

				//=========================================================================
				// Manejo de flags para la primera peticion
				if (evSet.flags & EV_FLAG0)
				{
					std::cout << "CON EV_FLAG0" << std::endl;
					EV_SET(&evSet, evList[i].ident, EVFILT_READ, EV_ADD & EV_FLAG0, 0, 0, NULL);
					kevent(kq, &evSet, 1, NULL, 0, NULL); // Agregar el evento modificado al conjunto de eventos
				}
				else if (evSet.flags | EV_FLAG0)
				{
					std::cout << "SIN EV_FLAG0" << std::endl;
				}

				// Colocar el evento en EVFILT_WRITE para enviar la respuesta
				EV_SET(&evSet, evList[i].ident, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
				kevent(kq, &evSet, 1, NULL, 0, NULL);

				//=================DESDE AQUI==============================================
				// Recibir datos del cliente
				memset(buffer, 0, sizeof(buffer));
				int n = recv(evList[i].ident, buffer, sizeof(buffer), 0);
				if (n < 0)
				{
					std::cerr << "Error al recibir datos: " << strerror(errno) << std::endl;
					close(evList[i].ident);
					continue; // Continuar con el siguiente intento de aceptar conexiones
				}
				//===================PETICION==============================================
				// TODO
				printPeticion(buffer);
				//===================PARSING==============================================
				HTTPRequest request(buffer);
				// TODO
				request.print();
				//=========================================================================

				// HTTPBody body(request);
				HTTPRes response(request, &_config);
				// TODO
				printResponse(response.getHeader(), response.getContent());

				_config.print();

				//=========================================================================
				_header = response.getHeader();
				_content = response.getContent();
				//=========================================================================
			}
			// Escribir en el socket cuando esté listo para escribir
			else if (evList[i].filter == EVFILT_WRITE)
			{
				std::cout << "ESTO ES EVFILT_WRITE" << std::endl;
				// Enviar la respuesta al cliente
				sendResGet(evList[i].ident, _header, _content);

				// Después de enviar la respuesta, eliminar el evento EVFILT_WRITE
				EV_SET(&evSet, evList[i].ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
				kevent(kq, &evSet, 1, NULL, 0, NULL);
			}
		}
	}
}

// 1. **Receive New Connection**:
//    - Este caso se maneja cuando se recibe una nueva conexión en el socket principal (`sockfd`).
//    - Se comprueba si la identificación (`ident`) del evento en `evList` es igual al identificador del socket principal (`sockfd`).
//    - Si es así, significa que hay una nueva conexión entrante.
//    - Se acepta la conexión utilizando la función `accept` y se asigna un nuevo descriptor de archivo (`fd`) para esta conexión.
//    - Se llama a `addConnet(fd)` para agregar esta conexión a la lista de clientes.
//    - Si la conexión se agregó correctamente, se registra el descriptor de archivo (`fd`) para eventos de lectura (`EVFILT_READ`) utilizando `kevent`.
//    - Si la conexión no se puede agregar, se imprime un mensaje de "connection refused" y se cierra el descriptor de archivo (`fd`).

// 2. **Client Disconnected**:
//    - Este caso se maneja cuando se detecta que un cliente se ha desconectado.
//    - Se comprueba si la bandera `EV_EOF` está activada en el evento en `evList`.
//    - Si la bandera `EV_EOF` está activada, significa que se ha detectado el fin de la conexión desde el lado del cliente.
//    - Se obtiene el descriptor de archivo (`fd`) correspondiente al cliente que se desconectó.
//    - Se elimina este descriptor de archivo de la lista de clientes utilizando `delConnect(fd)`.
//    - Se elimina el evento asociado a este descriptor de archivo de la lista de eventos utilizando `EV_DELETE`.
//    - Se imprime un mensaje indicando que el cliente se ha desconectado.

// 3. **Read Message from Client**:
//    - Este caso se maneja cuando se detecta que hay datos para leer desde un cliente.
//    - Se comprueba si el filtro del evento es `EVFILT_READ`.
//    - Si es así, significa que hay datos disponibles para leer en el descriptor de archivo (`fd`).
//    - Se lee el mensaje del cliente utilizando la función `recv` y se almacena en el búfer `buffer`.
//    - Se realiza cualquier procesamiento adicional necesario, como imprimir la petición recibida (`printPeticion`), analizar la solicitud HTTP (`HTTPRequest`), generar una respuesta HTTP (`HTTPRes`), y así sucesivamente.
//    - Finalmente, se envía la respuesta al cliente utilizando la función `sendResGet` con los encabezados y contenido generados.
