
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <vector>
#include <sys/stat.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/event.h>

#define MAX_MSG_SIZE 1024
#define MAX_EVENTS 5
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\x1B[38;2;47;82;191m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define ORANGE "\033[1;31m"
#define WHITE "\033[0;37m"
#define BLACK "\x1B[30m"
#define RESET "\x1B[0m"

class HTTPRequest
{
private:
	bool _last;
	// char *_byteContent;
	std::string _fileType;
	std::string _fileName;
	std::string _boundary;
	std::string _header;
	std::map<std::string, std::string> _map;
	// HTTPRequest &operator=(const HTTPRequest &);

public:
	HTTPRequest();
	HTTPRequest(const HTTPRequest &);
	~HTTPRequest();

	void setMulti(bool &);
	std::string getFileName();
	std::string getFileType();
	void findFileName(const char *);
	void takeHeader(std::stringstream &);
	void checkFirstBoundary(std::vector<std::string> &);
	void checkLastBoundary(std::vector<std::string> &);
	void getBuffer(const char *);

	const std::map<std::string, std::string> &getMap() const;
	const std::string &getHeader(const std::string name) const;
	std::string &getHeader();

	void print();
	void cleanObject();
	std::string getContent();
};

int kq;
int sockfd;

HTTPRequest::HTTPRequest() : _last(false) {}

HTTPRequest::HTTPRequest(const HTTPRequest &rhs)
{
	_last = rhs._last;
	_fileName = rhs._fileName;
	_boundary = rhs._boundary;
	_map = rhs._map;
}

HTTPRequest::~HTTPRequest() {}

void HTTPRequest::setMulti(bool &multi) { multi = _last; }

std::string HTTPRequest::getFileName() { return _fileName; }

std::string HTTPRequest::getFileType() { return _fileType; }

void HTTPRequest::findFileName(const char *buf)
{
	std::string input(buf);
	if (input.find("filename=") != std::string::npos)
	{
		_fileName = input.substr(input.find("filename=") + 10,
								 input.find_last_of("\"") - (input.find("filename=") + 10));
		_fileType = _fileName.substr(_fileName.find_last_of(".") + 1,
									 _fileName.length() - (_fileName.find(".") + 1) - 1);
		// std::cout << RED "Filename: -" << _fileName << "- FileType: -" << _fileType << "-" RESET << std::endl;
	}
}

void HTTPRequest::takeHeader(std::stringstream &ss)
{
	int i = 0;
	std::string first, el, line;
	std::getline(ss, first);
	std::stringstream ssFirst(first);
	while (getline(ssFirst, el, ' '))
	{
		if (!el.empty() && el.back() == '\r')
			el.pop_back();
		switch (i)
		{
		case 0:
			_map["Method"] = el;
			break;
		case 1:

			_map["Path"] = el;
			break;
		case 2:
			_map["Version"] = el;
			break;
		}
		i++;
	}
	while (getline(ss, line))
	{
		if (line.length() == 1)
			break;
		size_t pos = line.find(':');
		if (pos != std::string::npos)
		{
			std::string name = line.substr(0, pos);
			std::string val = line.substr(pos + 2);
			_map[name] = val;
		}
	}
	for (std::map<std::string, std::string>::iterator it = _map.begin(); it != _map.end(); it++)
	{
		if (it->first.find("Content-Type") != std::string::npos &&
			it->second.find("multipart") != std::string::npos)
		{
			_boundary = it->second.substr(it->second.find("boundary") + 9, it->second.length() - (it->second.find("boundary") + 9) - 1);
			std::cout << RED << "Boundary -> -" << _boundary << RESET << std::endl;
			_last = true;
			break;
		}
	}
}

void HTTPRequest::checkFirstBoundary(std::vector<std::string> &content)
{
	if (content[0].find(_boundary) == std::string::npos)
		return;
	int count = 0;
	for (std::vector<std::string>::iterator it = content.begin(); it != content.end() && count < 4;)
	{
		// if ((*it).find("filename") != std::string::npos)
		// 	_fileName = (*it).substr((*it).find("filename") + 10, (*it).find_last_of("\"") - ((*it).find("filename") + 10) - 1);
		content.erase(it);
		count++;
	}
}

void HTTPRequest::checkLastBoundary(std::vector<std::string> &content)
{
	for (std::vector<std::string>::iterator it = content.begin(); it != content.end();)
	{
		if ((*it).find(_boundary) != std::string::npos)
		{
			_last = false;
			content.erase(it);
			if (it == content.end())
				break;
			continue;
		}
		it++;
	}
	std::string mapContent;
	for (std::vector<std::string>::iterator it = content.begin(); it != content.end(); it++)
		mapContent += *it;
	_map["Content"] = mapContent;
}

void HTTPRequest::getBuffer(const char *buf)
{
	std::string line, input(buf);
	std::stringstream ss(input);
	std::vector<std::string> content;
	// if (!multi)
	// {
	findFileName(buf);
	takeHeader(ss);
	// std::cout << RED << "Print header:\n";
	// for (std::map<std::string, std::string>::iterator it = _map.begin(); it != _map.end(); it++)
	// 	std::cout << "Key: " << it->first << " Value: " << it->second << std::endl;
	// std::cout << RESET << std::endl;
	while (getline(ss, line))
		content.push_back(line + "\n");
	if (content.size() > 0)
	{
		// if (_last)
		// 	checkFirstBoundary(content);
		// _boundary.append("--");
		checkLastBoundary(content);
	}
	// }
	// else
	// {
	// 	// std::cout << RED << "Print header:\n";
	// 	// for (std::map<std::string, std::string>::iterator it = _map.begin(); it != _map.end(); it++)
	// 	// 	std::cout << "Key: " << it->first << " Value: " << it->second << std::endl;
	// 	// std::cout << RESET << std::endl;
	// 	while (getline(ss, line))
	// 		content.push_back(line + "\n");
	// 	checkFirstBoundary(content);
	// 	checkLastBoundary(content);
	// }
	content.clear();
	std::cout << RED << "Content: " << std::endl;
	std::cout << _map["Content"] << std::endl;
	_map["Content"] = std::string("<!DOCTYPE html>\r\n<html>\r\n\r\n<head>\r\n<meta charset=\"UTF-8\">\r\n<title>vduchi</title>\r\n</head>\r\n\r\n<body>\r\n\r\n<h2>Upload File</h2>\r\n<form action=\"../cgi_bin/upload.py\" method=\"post\" enctype=\"multipart/form-data\">\r\n"
								  "<label for=\"file\">Choose file:</label>\r\n<input type=\"file\" id=\"fileid\" name=\"filename\">\r\n<input type=\"submit\" value=\"Upload\">\r\n</form>\r\n<br>\r\n<br>\r\n<h2>New User</h2>");
	_map["Content"] += "<form action=\"../cgi_bin/register.py\" method=\"post\">\r\n<label for=\"nombre\">Nombre:</label>\r\n<input type=\"text\" id=\"nombre\" name=\"nombre\"><br><br>\r\n";
	_map["Content"] += "<label for=\"email\">Email:</label>\r\n<input type=\"email\" id=\"email\" name=\"email\"><br><br>\r\n<input type=\"submit\" value=\"Enviar\">\r\n\r\n</body>\r\n\r\n</html> ";
	std::cout << RED << "Content: " << std::endl;
	std::cout << _map["Content"] << std::endl;
}

const std::map<std::string, std::string> &HTTPRequest::getMap() const { return _map; }

const std::string &HTTPRequest::getHeader(const std::string name) const { return _map.find(name)->second; }

std::string &HTTPRequest::getHeader()
{
	// if (!res._content.empty())
	// 	res._header.addOne(res._request.getHeader("Version"), "200 OK");
	// else
	// 	res._header.addOne(res._request.getHeader("Version"), "404 Not Found");

	// // // esto es solo para la raiz
	// // if (path == "/")
	// res._header.addField("Content-Type", "text/html; charset=UTF-8");
	// // else
	// // 	// res._header.addField("Content-Type", mime.getContentType(extension));
	// // 	res._header.addField("Content-Type", res._config.getContentType(extension));

	// res._header.addField("Content-Length", std::to_string(res._content.length()));
	// res._header.addField("Cookie", res._request.getHeader("Cookie"));
	// // res._header.addField("Date", DateField());
	// // res._header.addField("ETag", generateETag(res._content));
	// res._header.addField("Server", "ACME co.");
	// res._header.addField("42-Barcelona", "nmota-bu, vduchi");
	int len = _map["Content"].length();
	std::stringstream ss;
	ss << len;
	std::string l = ss.str();
	_header += "HTTP/1.1 200 OK\r\n";
	_header += "Content-Type: text/html; charset=UTF-8\r\n";
	_header += "Content-Length: " + l;
	_header += "\r\nETag: a250d446db79663a\r\n";
	_header += "Date: Wed, 29 May 2024 21:04:42 GMT\r\n";
	_header += "Server: ACME co.\r\n";
	_header += "42-Barcelona: nmota-bu, vduchi\r\n";
	return _header;
}

void HTTPRequest::print()
{
	std::cout << RED << "[ Request client ]" << std::endl;
	for (std::map<std::string, std::string>::iterator it = _map.begin(); it != _map.end(); it++)
	{
		if (it->first == "Content")
			continue;
		std::cout << RED << it->first << ": " << GREEN << it->second << std::endl;
	}
	std::cout << RESET;
}

std::string HTTPRequest::getContent()
{
	return _map["Content"];
}

void HTTPRequest::cleanObject()
{
	_map.clear();
	_boundary.clear();
	_fileName.clear();
}

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

void createSocket()
{
	struct addrinfo *addr; // informacion sobre direcciones
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints)); // bzero to hints
	hints.ai_flags = AI_PASSIVE;	  // se especifica que debe devolver direcciones par vincular al socket de escucha
	hints.ai_family = PF_UNSPEC;	  // IPv4 o IPv6
	hints.ai_socktype = SOCK_STREAM;  // tipo del socket
	getaddrinfo("127.0.0.1", "8080", &hints, &addr);
	sockfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);

	if (bind(sockfd, addr->ai_addr, addr->ai_addrlen) < 0)
	{
		std::string errorMsg = "Socket binding error:\n";
		errorMsg += "Port: " + std::to_string(ntohs((reinterpret_cast<sockaddr_in *>(addr->ai_addr))->sin_port)) + "\n";
		errorMsg += strerror(errno);
		throw std::runtime_error(errorMsg);
	}

	listen(sockfd, 5);

	std::cout << "Server online.\nlistein port: " << ntohs((reinterpret_cast<sockaddr_in *>(addr->ai_addr))->sin_port) << std::endl;
	std::cout << "Servidor esperando conexiones..." << std::endl;
}

void createKqueue()
{
	kq = kqueue();
	if (kq == -1)
	{
		std::cerr << "Error creating kqueue: " << strerror(errno) << std::endl;
		throw std::runtime_error("creating kqueue");
	}
}

struct client_data
{
	int fd;
	std::string path;
	std::string prePath;
} clients[MAX_EVENTS];

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

void run(int sockfd, int kq)
{
	char buffer[MAX_MSG_SIZE];
	struct kevent evSet;
	struct kevent evList[MAX_EVENTS];
	struct sockaddr_storage addr;
	socklen_t socklen = sizeof(addr);

	HTTPRequest request;

	std::string _header;
	std::string _content;

	int flags = EV_ADD | EV_ENABLE | EV_FLAG0; // Poner EV_FLAG1 si hay multipart

	while (42)
	{
		// bool checkEVFlag = false;
		int num_events = kevent(kq, NULL, 0, evList, MAX_EVENTS, NULL);

		for (int i = 0; i < num_events; i++)
		{
			std::cout << MAGENTA "Enter for" RESET << std::endl;
			// receive new connection
			if (evList[i].ident == (unsigned long)sockfd)
			{
				int fd = accept(evList[i].ident, (struct sockaddr *)&addr, &socklen);
				if (addConnect(fd) == 0)
				{
					// EV_FLAG0 PAR LA PRIMERA PETICION
					// EV_SET(&evSet, fd, EVFILT_READ, EV_ADD | EV_FLAG0 , 0, 0, NULL);
					EV_SET(&evSet, fd, EVFILT_READ, flags, 0, 0, NULL);
					kevent(kq, &evSet, 1, NULL, 0, NULL);
					// _ref = false;
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
				std::cout << "\nReceived " << n << " bytes\n"
						  << buffer << std::endl
						  << std::endl;
				request.getBuffer(buffer);
				_content = request.getContent();
				_header = request.getHeader();
				//=========================================================================
				if (evSet.flags & EV_FLAG0)
				{
					// 	std::cout << "CON EV_FLAG0" << std::endl;
					flags &= ~EV_FLAG0; // Eliminar EV_FLAG0

					// 	// EV_SET(&evSet, evList[i].ident, EVFILT_READ, EV_ADD & EV_FLAG0, 0, 0, NULL);
					EV_SET(&evSet, evList[i].ident, EVFILT_WRITE, flags, 0, 0, NULL);
					kevent(kq, &evSet, 1, NULL, 0, NULL); // Agregar el evento modificado al conjunto de eventos
				}
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
			std::cout << MAGENTA "Exit for" RESET << std::endl;
		}
	}
}

int main(int argc, char **argv)
{
	try
	{
		createKqueue();
		createSocket();

		// // Configurar evento para el socket de escucha
		struct kevent eventSet;
		EV_SET(&eventSet, sockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
		kevent(kq, &eventSet, 1, NULL, 0, NULL);

		run(sockfd, kq);
		// config.print();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
