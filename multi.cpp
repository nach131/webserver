#include <iostream>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/event.h>
#include <vector>

#define MAX_EVENTS 1024
#define MAX_MSG_SIZE 4096
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

struct client_data {
    int fd;
    std::string path;
    std::string prePath;
} clients[MAX_EVENTS];

class HTTPRequest {
    // Implementación de la clase HTTPRequest
};

class HTTPRes {
    // Implementación de la clase HTTPRes
public:
    HTTPRes(const HTTPRequest& request, ServerConfig* config, bool& ref, bool& write, bool& send)
        : _request(request), _config(config), _ref(ref), _write(write), _send(send) {}

    std::string getHeader() {
        // Implementación del método getHeader
        return "";
    }

    std::string getContent() {
        // Implementación del método getContent
        return "";
    }

private:
    const HTTPRequest& _request;
    ServerConfig* _config;
    bool& _ref;
    bool& _write;
    bool& _send;
};

class ServerConfig {
public:
    int getServerSocket() const {
        // Implementación del método getServerSocket
        return 0;
    }
};

class AdminServer {
public:
    void run(const std::vector<int>& sockfds, int kq);

private:
    int _flags;
    bool _write;
    bool _multi;
    bool _send;
    bool _ref;
    std::string _header;
    std::string _content;
    ServerConfig _config;

    int getConnect(int fd);
    int addConnect(int fd);
    int delConnect(int fd);
    void sendResGet(int ident, const std::string& header, const std::string& content);
    void printPeticion(const char* buffer);
};

int AdminServer::getConnect(int fd) {
    for (int i = 0; i < MAX_EVENTS; i++)
        if (clients[i].fd == fd)
            return i;
    return -1;
}

int AdminServer::addConnect(int fd) {
    if (fd < 1)
        return -1;
    int i = getConnect(0);
    if (i == -1)
        return -1;
    clients[i].fd = fd;
    return 0;
}

int AdminServer::delConnect(int fd) {
    if (fd < 1)
        return -1;
    int i = getConnect(fd);
    if (i == -1)
        return -1;
    clients[i].fd = 0;
    return close(fd);
}

void AdminServer::run(const std::vector<int>& sockfds, int kq) {
    HTTPRequest request;
    char buffer[MAX_MSG_SIZE];

    _write = false;
    _multi = false;
    _send = false;

    struct kevent evSet;
    struct kevent evList[MAX_EVENTS];

    _flags = EV_ADD | EV_FLAG0; // Poner EV_FLAG1 si hay multipart

    // Añadir todos los sockets de servidor al kqueue
    for (size_t i = 0; i < sockfds.size(); ++i) {
        EV_SET(&evSet, sockfds[i], EVFILT_READ, EV_ADD, 0, 0, NULL);
        kevent(kq, &evSet, 1, NULL, 0, NULL);
    }

    while (true) {
        int num_events = kevent(kq, NULL, 0, evList, MAX_EVENTS, NULL);

        for (int i = 0; i < num_events; i++) {
            bool isServerSocket = false;
            for (size_t j = 0; j < sockfds.size(); ++j) {
                if (evList[i].ident == static_cast<unsigned long>(sockfds[j])) {
                    isServerSocket = true;
                    break;
                }
            }

            if (isServerSocket) {
                int fd = accept(evList[i].ident, NULL, NULL);
                if (addConnect(fd) == 0) {
                    EV_SET(&evSet, fd, EVFILT_READ, _flags, 0, 0, NULL);
                    kevent(kq, &evSet, 1, NULL, 0, NULL);
                    _ref = true;
                } else {
                    std::cout << "connection refused." << std::endl;
                    close(fd);
                }
            } else if (evList[i].flags & EV_EOF) {
                int fd = evList[i].ident;
                std::cout << "client #" << getConnect(fd) << " disconnected." << std::endl;
                EV_SET(&evSet, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                kevent(kq, &evSet, 1, NULL, 0, NULL);
                delConnect(fd);
            } else if (evList[i].filter == EVFILT_READ) {
                std::cout << RED << " [=================EVFILT_READ=================]" << std::endl;
                std::cout << "_write: " << _write << RESET << std::endl;
                _send = false;

                memset(buffer, 0, sizeof(buffer));
                int n = recv(evList[i].ident, buffer, sizeof(buffer), 0);
                if (n < 0) {
                    std::cerr << "Error al recibir datos: " << strerror(errno) << std::endl;
                    close(evList[i].ident);
                    continue;
                }
                printPeticion(buffer);
                request.takeBuffer(buffer, n, _multi, _write);
                request.print();

                HTTPRes response(request, &_config, _ref, _write, _send);

                if (evSet.flags & EV_FLAG0) {
                    std::cout << "CON EV_FLAG0" << std::endl;
                    _flags &= ~EV_FLAG0;

                    EV_SET(&evSet, evList[i].ident, EVFILT_READ, _flags, 0, 0, NULL);
                    kevent(kq, &evSet, 1, NULL, 0, NULL);
                    _ref = true;
                }

                EV_SET(&evSet, evList[i].ident, EVFILT_WRITE, _flags, 0, 0, NULL);
                kevent(kq, &evSet, 1, NULL, 0, NULL);

                _header = response.getHeader();
                _content = response.getContent();

                if (!_multi)
                    request.cleanObject();
            } else if (evList[i].filter == EVFILT_WRITE) {
                std::cout << YELLOW;
                std::cout << "ESTO ES EVFILT_WRITE" << std::endl;

                _multi ? (_send ? sendResGet(evList[i].ident, _header, _content) : void()) : sendResGet(evList[i].ident, _header, _content);

                std::cout << YELLOW << " [=================EVFILT_WRITE=================]" << std::endl;
                std::cout << " _write: " << _write << std::endl;

                if (_write) {
                    _content.clear();
                    _header.clear();
                }
                int flags_tmp = evSet.flags;

                EV_SET(&evSet, evList[i].ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
                kevent(kq, &evSet, 1, NULL, 0, NULL);
                evSet.flags = flags_tmp;
                std::cout << RESET;
            }
        }
    }

    close(_config.getServerSocket());
}
