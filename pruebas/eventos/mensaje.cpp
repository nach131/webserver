#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/event.h>

#define NUM_CLIENTS 10
#define MAX_EVENTS 32
#define MAX_MSG_SIZE 256

struct client_data
{
	int fd;
} clients[NUM_CLIENTS];

int get_conn(int fd)
{
	for (int i = 0; i < NUM_CLIENTS; i++)
		if (clients[i].fd == fd)
			return i;
	return -1;
}

int conn_add(int fd)
{
	if (fd < 1)
		return -1;
	int i = get_conn(0);
	if (i == -1)
		return -1;
	clients[i].fd = fd;
	return 0;
}

int conn_del(int fd)
{
	if (fd < 1)
		return -1;
	int i = get_conn(fd);
	if (i == -1)
		return -1;
	clients[i].fd = 0;
	return close(fd);
}

void recv_msg(int s)
{
	char buf[MAX_MSG_SIZE];
	int bytes_read = recv(s, buf, sizeof(buf) - 1, 0);
	buf[bytes_read] = 0;
	std::cout << "client #" << get_conn(s) << ": " << buf;
	std::cout.flush();

	// Construimos el mensaje de confirmación
	char confirm_msg[MAX_MSG_SIZE];
	snprintf(confirm_msg, sizeof(confirm_msg), "Hemos recibido su mensaje: %s\n", buf);

	// Enviamos el mensaje de confirmación al cliente
	send(s, confirm_msg, strlen(confirm_msg), 0);
}

void send_welcome_msg(int s)
{
	char msg[80];
	snprintf(msg, sizeof(msg), "welcome! you are client #%d!\n", get_conn(s));
	send(s, msg, strlen(msg), 0);
}

void run_event_loop(int kq, int local_s)
{
	struct kevent evSet;
	struct kevent evList[MAX_EVENTS];
	struct sockaddr_storage addr;
	socklen_t socklen = sizeof(addr);

	std::cout << "Server online\nlistein port:8082\n[ nc 127.0.0.1 8082 ]\n";

	while (1)
	{
		int num_events = kevent(kq, NULL, 0, evList, MAX_EVENTS, NULL);
		for (int i = 0; i < num_events; i++)
		{
			// receive new connection
			if (evList[i].ident == local_s)
			{
				int fd = accept(evList[i].ident, (struct sockaddr *)&addr, &socklen);
				if (conn_add(fd) == 0)
				{
					EV_SET(&evSet, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
					kevent(kq, &evSet, 1, NULL, 0, NULL);
					send_welcome_msg(fd);
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
				std::cout << "client #" << get_conn(fd) << " disconnected." << std::endl;
				EV_SET(&evSet, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
				kevent(kq, &evSet, 1, NULL, 0, NULL);
				conn_del(fd);
			} // read message from client
			else if (evList[i].filter == EVFILT_READ)
			{
				recv_msg(evList[i].ident);
			}
		}
	}
}

int create_socket_and_listen()
{
	struct addrinfo *addr; // informacion sobre direcciones
	struct addrinfo hints;
	memset(&hints, 0, sizeof hints); // bzero to hints
	hints.ai_flags = AI_PASSIVE;	 // se especifica que debe devolver direcciones par vincular al socket de escucha
	hints.ai_family = PF_UNSPEC;	 // IPv4 o IPv6
	hints.ai_socktype = SOCK_STREAM; // tipo del socket
	getaddrinfo("127.0.0.1", "8082", &hints, &addr);
	int sockfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	bind(sockfd, addr->ai_addr, addr->ai_addrlen);
	listen(sockfd, 5);
	return sockfd;
}

int main(int argc, char *argv[])
{
	int sockfd = create_socket_and_listen();

	int kq = kqueue();
	struct kevent evSet;
	EV_SET(&evSet, sockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
	kevent(kq, &evSet, 1, NULL, 0, NULL);

	run_event_loop(kq, sockfd);
	return EXIT_SUCCESS;
}

// nc 127.0.0.1 8082
