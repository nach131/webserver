
void AdminServer::run(int sockfd, int kq)
{
	_multi = false;
	HTTPRequest request;
	char buffer[MAX_MSG_SIZE];
	struct kevent evSet;
	struct kevent evList[MAX_EVENTS];
	struct sockaddr_storage addr;
	socklen_t socklen = sizeof(addr);

	_flags = EV_ADD | EV_FLAG0; // Poner EV_FLAG1 si hay multipart

	while (42)
	{
		// bool checkEVFlag = false;
		int num_events = kevent(kq, NULL, 0, evList, MAX_EVENTS, NULL);

		for (int i = 0; i < num_events; i++)
		{
			// receive new connection
			if (evList[i].ident == (unsigned long)sockfd)
			{
				int fd = accept(evList[i].ident, (struct sockaddr *)&addr, &socklen);
				if (addConnect(fd) == 0)
				{

					EV_SET(&evSet, fd, EVFILT_READ, _flags, 0, 0, NULL);
					kevent(kq, &evSet, 1, NULL, 0, NULL);
					// _ref = false;
					_ref = true;
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
				// cliente peticion
				printPeticion(buffer);
				//===================PARSING==============================================
				// HTTPRequest request(buffer);
				request.getBuffer(buffer, _multi);
				// [ Request client ]
				request.print();
				//=========================================================================

				// HTTPBody body(request);
				HTTPRes response(request, &_config, _ref);
				// TODO
				// printResponse(response.getHeader(), response.getContent());

				// _config.print();
				// //=========================================================================

				// Manejo de flags para la primera peticion
				if (evSet.flags & EV_FLAG1)
				{
					std::cout << " CON EV_FLAG1" << std::endl;
				}
				else if (evSet.flags | EV_FLAG1)
				{
					std::cout << " SIN EV_FLAG1" << std::endl;
				}

				if (evSet.flags & EV_FLAG0)
				{
					std::cout << "CON EV_FLAG0" << std::endl;
					_flags &= ~EV_FLAG0; // Eliminar EV_FLAG0

					// EV_SET(&evSet, evList[i].ident, EVFILT_READ, EV_ADD & EV_FLAG0, 0, 0, NULL);
					EV_SET(&evSet, evList[i].ident, EVFILT_READ, _flags, 0, 0, NULL);
					kevent(kq, &evSet, 1, NULL, 0, NULL); // Agregar el evento modificado al conjunto de eventos
														  // checkEVFlag = true;
					_ref = true;
				}
				else if (evSet.flags | EV_FLAG0)
				{
					std::cout << "SIN EV_FLAG0" << std::endl;
				}

				// Colocar el evento en EVFILT_WRITE para enviar la respuesta
				// TODO controlar si es multipart y si ha acabado de enviar
				EV_SET(&evSet, evList[i].ident, EVFILT_WRITE, _flags, 0, 0, NULL);
				kevent(kq, &evSet, 1, NULL, 0, NULL);

				//=========================================================================
				_header = response.getHeader();
				_content = response.getContent();

				// //========================VALERIO===========================================
				// // TODO Poner este codigo comentado en la primera peticion
				// if (!_multi)
				// {
				// 	_header = response.getHeader();
				// 	_content = response.getContent();
				// }
				// else
				// 	_content.append(response.getContent());
				// request.setMulti(_multi);
				// std::cout << GREEN "Multi: " << _multi << RESET << std::endl;

				// //=========================================================================
				// if (evSet.flags & EV_FLAG0)
				// {
				// 	std::cout << "CON EV_FLAG0" << std::endl;
				// 	_flags &= ~EV_FLAG0; // Eliminar EV_FLAG0

				// 	// EV_SET(&evSet, evList[i].ident, EVFILT_READ, EV_ADD & EV_FLAG0, 0, 0, NULL);
				// 	EV_SET(&evSet, evList[i].ident, EVFILT_READ, _flags, 0, 0, NULL);
				// 	kevent(kq, &evSet, 1, NULL, 0, NULL); // Agregar el evento modificado al conjunto de eventos
				// 	_ref = false;
				// }
				// else
				// {
				// 	std::cout << "SIN EV_FLAG0" << std::endl;
				// }
				// //=======================VALERIO==================================================
				// // Manejo de flags para la primera peticion
				// if (_multi)
				// {
				// 	std::cout << "CON EV_FLAG1" << std::endl;
				// 	int flags_tmp = evSet.flags;
				// 	flags_tmp |= ~EV_FLAG1; // Eliminar EV_FLAG1

				// 	// EV_SET(&evSet, evList[i].ident, EVFILT_READ, EV_ADD & EV_FLAG0, 0, 0, NULL);
				// 	EV_SET(&evSet, evList[i].ident, EVFILT_READ, flags_tmp, 0, 0, NULL);
				// 	kevent(kq, &evSet, 1, NULL, 0, NULL); // Agregar el evento modificado al conjunto de eventos
				// }
				// else
				// {
				// 	std::cout << "SIN EV_FLAG1" << std::endl;
				// 	int flags_tmp = evSet.flags;
				// 	flags_tmp &= ~EV_FLAG1; // Eliminar EV_FLAG1

				// 	// EV_SET(&evSet, evList[i].ident, EVFILT_READ, _flags, 0, 0, NULL);
				// 	// kevent(kq, &evSet, 1, NULL, 0, NULL); // Agregar el evento modificado al conjunto de eventos
				// 	// Colocar el evento en EVFILT_WRITE para enviar la respuesta
				// 	EV_SET(&evSet, evList[i].ident, EVFILT_WRITE, flags_tmp, 0, 0, NULL);
				// 	kevent(kq, &evSet, 1, NULL, 0, NULL);
				// }
				// //=========================================================================
				// // EV_SET(&evSet, evList[i].ident, EVFILT_WRITE, _flags, 0, 0, NULL);
				// // kevent(kq, &evSet, 1, NULL, 0, NULL);
				// //=========================================================================
				// if (!_multi)
				// 	request.cleanObject();
			}
			// Escribir en el socket cuando esté listo para escribir
			else if (evList[i].filter == EVFILT_WRITE)
			{
				std::cout << "ESTO ES EVFILT_WRITE" << std::endl;
				// Enviar la respuesta al cliente
				sendResGet(evList[i].ident, _header, _content);

				int flags_tmp = evSet.flags; // para guardar los flags activos

				// Después de enviar la respuesta, eliminar el evento EVFILT_WRITE
				EV_SET(&evSet, evList[i].ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
				kevent(kq, &evSet, 1, NULL, 0, NULL);
				evSet.flags = flags_tmp; // asignamos los flags activos
			}
		}
	}

	// Cerrar el socket del servidor (esto no se alcanzará)
	close(_config.getServerSocket());
}