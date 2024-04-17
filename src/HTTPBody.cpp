/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPBody.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 12:28:42 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/17 17:06:59 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MIME.hpp"
#include "HTTPBody.hpp"
#include "Colors.hpp"

// GET, POST, DELETE

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

	std::cout << "==========GET===========\n";
	std::cout << RED << "path: " << path << std::endl;
	std::cout << "La extensión del archivo es: " << extension << RESET << std::endl;
	std::cout << "========================\n";

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

HTTPBody::HTTPBody(const ClientParsing &pars)
{
	std::map<std::string, int (*)(HTTPBody &client, std::string)> map;

	map["GET"] = &methodGet;
	map["POST"] = &methodPost;
	map["DELETE"] = &methodDelete;

	std::map<std::string, int (*)(HTTPBody &client, std::string)>::iterator it = map.find(pars.getMethod());

	if (it != map.end())
		it->second(*this, pars.getPath());
	else
		// TODO
		// hacer funcion para devolver error 405 en header
		std::cout << "NotFoud: " << pars.getMethod() << std::endl;
}

HTTPBody::~HTTPBody() {}

void HTTPBody::setMIME(const std::string &mime) { _mime = mime; }

std::string const &HTTPBody::content() const
{
	return _content;
}

std::string const &HTTPBody::code() const { return _code; }

std::string const &HTTPBody::mime() const { return _mime; }
