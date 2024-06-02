/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AdminServer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:49:47 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/06/02 13:37:25 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AdminServer.hpp"

AdminServer::AdminServer(std::vector<ServerConfig *> &config) : _config(config) {}

AdminServer::~AdminServer() {}

struct client_data
{
	int fd;
} clients[MAX_EVENTS];

struct client_events
{
	struct kevent evSet;
	struct kevent evList;
	struct sockaddr_storage addr;
	socklen_t socklen;
} events[MAX_EVENTS];

static int getConnect(int fd)
{
	for (int i = 0; i < MAX_EVENTS; i++)
		if (clients[i].fd == fd)
			return i;
	return -1;
}

static int addConnect(int fd)
{
	if (fd < 1)
		return -1;
	int i = getConnect(0);
	if (i == -1)
		return -1;
	clients[i].fd = fd;
	return 0;
}

static int delConnect(int fd)
{
	if (fd < 1)
		return -1;
	int i = getConnect(fd);
	if (i == -1)
		return -1;
	clients[i].fd = 0;
	return close(fd);
}

void sendResGet(int newsockfd, const std::string &header, const std::string &content)
{
	int n;
	n = send(newsockfd, header.c_str(), header.length(), 0);
	if (n < 0)
	{
		std::string err("Error sending HTML header");
		close(newsockfd);
		Log::error(GET, err);
	}
	n = send(newsockfd, content.c_str(), content.length(), 0);
	if (n < 0)
	{
		std::string err("Error sending header");
		close(newsockfd);
		Log::error(GET, err);
	}
	Log::info(GET, "200 OK");
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
	void *data;
	char buffer[MAX_MSG_SIZE];
	HTTPRequest request[MAX_EVENTS];

	_write = false;
	_multi = false;
	_send = false;

	struct kevent evList[MAX_EVENTS];
	memset(evList, 0, sizeof(evList));
	memset(events, 0, sizeof(events));
	for (int i = 0; i < MAX_EVENTS; i++)
		events[i].socklen = sizeof(events[i].addr);

	_flags = EV_ADD | EV_FLAG0;

	while (42)
	{
		int num_events = kevent(kq, NULL, 0, evList, MAX_EVENTS, NULL);

		for (int i = 0; i < num_events; i++)
		{
			try
			{
				int j = takeServer(evList[i]);
				events[j].evList = evList[i];
				data = evList[i].udata;
				if (evList[i].ident == (unsigned long)_config[j]->getServerSocket())
				{
					int fd = accept(events[j].evList.ident, (struct sockaddr *)&events[j].addr, &events[j].socklen);
					if (addConnect(fd) == 0)
					{
						EV_SET(&events[j].evSet, fd, EVFILT_READ, _flags, 0, 0, data);
						kevent(kq, &events[j].evSet, 1, NULL, 0, NULL);
						_ref = true;
					}
					else
					{
						close(fd);
						Log::error(ADM, "Connection refused!");
					}
				}
				else if (events[j].evList.flags & EV_EOF)
				{
					int fd = events[j].evList.ident;
					EV_SET(&events[j].evSet, fd, EVFILT_READ, EV_DELETE, 0, 0, data);
					kevent(kq, &events[j].evSet, 1, NULL, 0, NULL);
					delConnect(fd);
					Log::info(ADM, "Connection closed");
				}
				else if (events[j].evList.filter == EVFILT_READ)
				{
					_send = false;
					memset(buffer, 0, sizeof(buffer));
					int n = recv(events[j].evList.ident, buffer, sizeof(buffer), 0);
					if (n < 0)
					{
						close(events[j].evList.ident);
						Log::error(ADM, "Error receiving data!");
					}
					request[j].takeBuffer(buffer, n, _multi, _write);
					HTTPRes response(request[j], _config[j], _ref, _write, _send);
					if (events[j].evSet.flags & EV_FLAG0)
					{
						_flags &= ~EV_FLAG0;
						EV_SET(&events[j].evSet, events[j].evList.ident, EVFILT_READ, _flags, 0, 0, data);
						kevent(kq, &events[j].evSet, 1, NULL, 0, NULL);
						_ref = true;
					}
					EV_SET(&events[j].evSet, events[j].evList.ident, EVFILT_WRITE, _flags, 0, 0, data);
					kevent(kq, &events[j].evSet, 1, NULL, 0, NULL);
					_header = response.getHeader();
					_content = response.getContent();
					if (!_multi)
						request[j].cleanObject();
				}
				else if (events[j].evList.filter == EVFILT_WRITE)
				{
					if (!_multi)
						sendResGet(events[j].evList.ident, _header, _content);
					else if (_multi && _send)
						sendResGet(events[j].evList.ident, _header, _content);
					if (_write)
					{
						_content.clear();
						_header.clear();
					}
					int flags_tmp = events[j].evSet.flags;
					EV_SET(&events[j].evSet, events[j].evList.ident, EVFILT_WRITE, EV_DELETE, 0, 0, data);
					kevent(kq, &events[j].evSet, 1, NULL, 0, NULL);
					events[j].evSet.flags = flags_tmp;
					std::cout << RESET;
				}
			}
			catch (const std::runtime_error &e)
			{
				// No necesito catch porque siguo con el loop igualmente
			}
		}
	}
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
