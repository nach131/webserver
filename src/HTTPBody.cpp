/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPBody.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 12:28:42 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/22 19:18:40 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MIME.hpp"
#include "HTTPBody.hpp"
#include "Colors.hpp"

// GET, POST, DELETE

// TODO
// PUEDE QUE NECESITE Accept-Language, PARA INCLUIR  charset=UTF-8 EN EL Content-Type
// SI QUE LO NECESITO
int HTTPBody::methodGet(HTTPBody &client, std::string path)
{
	std::string extension;
	MIME mime;

	size_t pos = path.find_last_of('.');
	if (pos != std::string::npos)
	{
		// Si se encuentra el carácter '.'Extrae la subcadena que contiene la extensión
		extension = path.substr(pos + 1);
		client.setMIME(mime.getMIME(extension));
	}

	std::ifstream file;
	if (path == "/")
		file.open("./dist/index.html");
	else
	{
		std::string filePath = "./dist" + path;
		file.open(filePath.c_str());
	}

	if (!file)
	{
		std::cerr << "Error al abrir el archivo " << path << std::endl;
		client._code = "404 Not Found";
	}
	// // Error 400: Solicitud incorrecta (si el archivo solicitado es un directorio en lugar de un archivo)
	// if (client._content.empty() && path.back() == '/')
	// 	client._code = "400 Bad Request";
	// // Error 401: No autorizado ( si el acceso al archivo está restringido)
	// if (client._content.empty() && !file.is_open())
	// 	client._code = "401 Unauthorized";
	else
	{
		std::ostringstream oss;
		oss << file.rdbuf();		 // Leer el contenido del archivo en un stringstream
		client._content = oss.str(); // Obtener el contenido como string
		client._code = "200 OK";
	}

	return 0;
}

int HTTPBody::methodPost(HTTPBody &client, std::string path)
{
	(void)client;
	std::cout << "==========POST==========\n";
	// std::cout << RED << "method: " << method << RESET << std::endl;
	std::cout << RED << "path: " << path << RESET << std::endl;
	std::cout << "========================\n";

	return 0;
}

int HTTPBody::methodDelete(HTTPBody &client, std::string path)
{
	(void)client;
	std::cout << "=========DELETE==========\n";
	// std::cout << RED << "method: " << method << RESET << std::endl;
	std::cout << RED << "path: " << path << RESET << std::endl;
	std::cout << "========================\n";
	return 0;
}

HTTPBody::HTTPBody(const HTTPRequest &request)
{
	std::map<std::string, int (*)(HTTPBody &client, std::string)> map;

	map["GET"] = &methodGet;
	map["POST"] = &methodPost;
	map["DELETE"] = &methodDelete;

	std::map<std::string, int (*)(HTTPBody &client, std::string)>::iterator it = map.find(request.getHeader("Method"));

	if (it != map.end())
		it->second(*this, request.getHeader("Path"));
	else
		// TODO
		// hacer funcion para devolver error 405 en header
		std::cout << "NotFoud: " << request.getHeader("Method") << std::endl;
}

HTTPBody::~HTTPBody() {}

void HTTPBody::setMIME(const std::string &mime) { _mime = mime; }

std::string const &HTTPBody::content() const
{
	return _content;
}

std::string const &HTTPBody::code() const { return _code; }

std::string const &HTTPBody::mime() const { return _mime; }
