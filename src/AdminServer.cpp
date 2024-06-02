/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AdminServer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:49:47 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/06/02 10:40:30 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AdminServer.hpp"

// #define NUM_CLIENTS 10
// #define MAX_EVENTS 32
// #define MAX_MSG_SIZE 8192

// EV_FLAG0 - to ref
// EV_FLAG1 - to multipas

// TODO Controlar que el proceso sea correcto, porque al
// recargar la pagina, a veces me pone "client disconnected" y
// "Error al enviar encabezado"

AdminServer::AdminServer(std::vector<ServerConfig *> &config) : _config(config) {}

AdminServer::~AdminServer() {}

void sendResGet(int newsockfd, const std::string &header, const std::string &content)
{
	std::cout << "[ sendResGet ]" << std::endl;

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
	std::cout << "[ CONTENT ]" << std::endl;
	std::cout << content << std::endl;
	std::cout << "========================" << RESET << std::endl;
}

//=========================================================================

struct client_data
{
	int fd;
	std::string path;
	std::string prePath;
} clients[MAX_EVENTS];

struct client_events
{
	struct kevent evSet;
	struct kevent evList;
	struct sockaddr_storage addr;
	socklen_t socklen;
} events[MAX_EVENTS];

int getConnect(int fd)
{
	for (int i = 0; i < MAX_EVENTS; i++)
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

int AdminServer::takeServer(struct kevent &event)
{
	int *p = (int *)event.udata;
	if (!p)
		return 0;
	int data = p[0];
	for (std::vector<ServerConfig *>::iterator it = _config.begin(); it != _config.end(); it++)
	{
		if ((*it)->getData() == data)
			return data;
	}
	return 0;
}

void AdminServer::run(int kq)
{
	HTTPRequest request[MAX_EVENTS];
	char buffer[MAX_MSG_SIZE];

	_write = false;
	_multi = false;
	_send = false;

	struct kevent evList[MAX_EVENTS];
	memset(evList, 0, sizeof(evList));
	memset(events, 0, sizeof(events));
	for (int i = 0; i < MAX_EVENTS; i++)
		events[i].socklen = sizeof(events[i].addr);

	_flags = EV_ADD | EV_FLAG0; // Poner EV_FLAG1 si hay multipart

	void *data;
	while (42)
	{
		int num_events = kevent(kq, NULL, 0, evList, MAX_EVENTS, NULL);

		for (int i = 0; i < num_events; i++)
		{
			std::cout << MAGENTA "Event number: " << num_events << " Ident: " << evList[i].ident << RESET << std::endl;
			int j = takeServer(evList[i]);
			std::cout << MAGENTA << "Server number: " << j << " Socket " << _config[j]->getServerSocket() << RESET << std::endl;
			// receive new connection
			events[j].evList = evList[i];
			data = evList[i].udata;
			if (evList[i].ident == (unsigned long)_config[j]->getServerSocket())
			{
				std::cout << MAGENTA "Accept connection " << j << RESET << std::endl;
				int fd = accept(events[j].evList.ident, (struct sockaddr *)&events[j].addr, &events[j].socklen);
				if (addConnect(fd) == 0)
				{
					EV_SET(&events[j].evSet, fd, EVFILT_READ, _flags, 0, 0, data);
					kevent(kq, &events[j].evSet, 1, NULL, 0, NULL);
					// _ref = false;
					_ref = true;
				}
				else
				{
					std::cout << "connection refused." << std::endl;
					close(fd);
				}
			} // client disconnected
			else if (events[j].evList.flags & EV_EOF)
			{
				std::cout << MAGENTA "End connection " << j << RESET << std::endl;
				int fd = events[j].evList.ident;
				// std::cout << "client #" << getConnect(fd) << " disconnected." << std::endl;
				EV_SET(&events[j].evSet, fd, EVFILT_READ, EV_DELETE, 0, 0, data);
				kevent(kq, &events[j].evSet, 1, NULL, 0, NULL);
				delConnect(fd);
			} // read message from client
			else if (events[j].evList.filter == EVFILT_READ)
			{
				std::cout << MAGENTA "Read connection " << j << RESET << std::endl;
				//=================DESDE AQUI==============================================
				// std::cout << RED << " [=================EVFILT_READ=================]" << std::endl;
				// std::cout << "_write: " << _write << RESET << std::endl;
				_send = false;

				// Recibir datos del cliente
				memset(buffer, 0, sizeof(buffer));
				int n = recv(events[j].evList.ident, buffer, sizeof(buffer), 0);
				if (n < 0)
				{
					std::cerr << "Error al recibir datos: " << strerror(errno) << std::endl;
					close(events[j].evList.ident);
					continue; // Continuar con el siguiente intento de aceptar conexiones
				}
				//===================PETICION==============================================
				// TODO
				// cliente peticion
				printPeticion(buffer);
				//===================PARSING==============================================
				// HTTPRequest request(buffer);
				request[j].takeBuffer(buffer, n, _multi, _write);
				// [ Request client ]
				request[j].print();

				//=========================================================================

				// HTTPBody body(request);
				HTTPRes response(request[j], _config[j], _ref, _write, _send);

				// //=========================================================================

				// Manejo de flags para la primera peticion
				if (events[j].evSet.flags & EV_FLAG0)
				{
					// std::cout << "CON EV_FLAG0" << std::endl;
					_flags &= ~EV_FLAG0; // Eliminar EV_FLAG0

					EV_SET(&events[j].evSet, events[j].evList.ident, EVFILT_READ, _flags, 0, 0, data);
					kevent(kq, &events[j].evSet, 1, NULL, 0, NULL);
					_ref = true;
				}

				EV_SET(&events[j].evSet, events[j].evList.ident, EVFILT_WRITE, _flags, 0, 0, data);
				kevent(kq, &events[j].evSet, 1, NULL, 0, NULL);

				//=========================================================================
				_header = response.getHeader();
				_content = response.getContent();

				if (!_multi)
					request[j].cleanObject();
			}
			// Escribir en el socket cuando esté listo para escribir
			else if (events[j].evList.filter == EVFILT_WRITE)
			{
				// std::cout << YELLOW;
				// std::cout << "ESTO ES EVFILT_WRITE" << std::endl;
				std::cout << MAGENTA "Write connection " << j << RESET << std::endl;
				if (!_multi)
					sendResGet(events[j].evList.ident, _header, _content);
				else if (_multi && _send)
					sendResGet(events[j].evList.ident, _header, _content);

				// std::cout << YELLOW << " [=================EVFILT_WRITE=================]" << std::endl;
				// std::cout << " _write: " << _write << std::endl;

				// TODO esto nunca pasas
				if (_write)
				{
					_content.clear();
					_header.clear();
				}
				int flags_tmp = events[j].evSet.flags; // para guardar los flags activos

				// Después de enviar la respuesta, eliminar el evento EVFILT_WRITE
				EV_SET(&events[j].evSet, events[j].evList.ident, EVFILT_WRITE, EV_DELETE, 0, 0, data);
				kevent(kq, &events[j].evSet, 1, NULL, 0, NULL);
				events[j].evSet.flags = flags_tmp; // asignamos los flags activos
				std::cout << RESET;
			}
		}
	}

	// Cerrar el socket del servidor (esto no se alcanzará)
	for (unsigned long i = 0; i < _config.size(); i++)
		close(_config[i]->getServerSocket());
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
