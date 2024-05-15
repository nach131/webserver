/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRes.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:54:23 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/15 17:48:10 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRes.hpp"

void HTTPRes::last()
{
	_header.addField("Cookie", _request.getHeader("Cookie"));

	_header.addField("Content-Length", std::to_string(_content.length()));
	_header.addField("ETag", generateETag(_content));

	_header.addField("Date", DateField());
	_header.addField("Server", "ACME co.");
	_header.addField("42-Barcelona", "nmota-bu, vduchi");
}

HTTPRes::HTTPRes(const HTTPRequest &request, ServerConfig *config) : _request(request), _config(config)
{
	std::string method = request.getHeader("Method");
	_location = _request.getLocation();

	if (method == "GET" && _config->isLocationAllowed(_location) && _config->isMethodAllowed(_location, "GET") && _config->isLocationOn(_location))
	{
		std::cout << "FILES EXPLORER" << std::endl;
		exploreFiles();
	}
	else if (method == "GET" && _config->isLocationAllowed(_location) && _config->isMethodAllowed(_location, "GET"))
		methodGet();
	else if (method == "POST" && _config->isMethodAllowed(_location, "POST"))
		methodPost();
	else if (method == "DELETE" && _config->isMethodAllowed(_location, "DELETE"))
		methodDelete();
	else
		methodErr();

	last();
}

HTTPRes::~HTTPRes() {}

void HTTPRes::rootPath()
{

	std::string path = _request.getHeader("Path");
	std::string extension = getExtension(path);

	_content = readFile("./dist/index.html");

	if (!_content.empty())
		_header.addOne(_request.getHeader("Version"), "200 OK");
	else
		_header.addOne(_request.getHeader("Version"), "404 Not Found");

	// esto es solo para la raiz
	_header.addField("Content-Type", "text/html; charset=UTF-8");
}

std::string const HTTPRes::OtherPath() const
{
	std::string path;
	if (_config->getFirst() == false)
		path = _config->getRootDirectory() + _request.getHeader("Path");
	else
	{
		std::string post = _request.getHeader("Path");
		path = _config->getPrePath() + post;
		// TODO hay que quitar los locates del pricipo de los errores
		// Error al abrir el archivo ./conf_web/error/404/files/script.js
	}
	std::cout << MAGENTA << path << RESET << std::endl;
	return path;
}

void HTTPRes::rootOtherFiles()
{
	// std::string path = _request.getHeader("Path");
	std::string path = OtherPath();

	std::string extension = getExtension(path);

	// _content = readFile(_config->getRootDirectory() + path);
	_content = readFile(path);

	// comprueba si el fichero a pasar tiene datos
	if (!_content.empty())
	{
		_header.addOne(_request.getHeader("Version"), "200 OK");
		_header.addField("Content-Type", _config->getContentType(extension));
	}
	else
		error404();
}

//=========================================================================

void HTTPRes::createContent(std::string filePath)
{
_content = readFile(filePath);

	// comprueba si el fichero a pasar tiene datos
	if (!_content.empty())
	{
		std::string extension = getExtension(filePath);
		_header.addOne(_request.getHeader("Version"), "200 OK");
		_header.addField("Content-Type", _config->getContentType(extension));
	}
	else
		error404();
}

void HTTPRes::folder()
{
	std::cout << "CARPETA" << std::endl;

	std::string index = _config->getIndex(_location);
	std::string filePath;

	if (index.empty())
		filePath = _config->getRoot(_location) + _request.getHeader("Path") + "index.html";
	else
		filePath = _config->getRoot(_location) + _request.getHeader("Path") + "/" + index;

	std::cout << index << std::endl;
	std::cout << filePath << std::endl;

	createContent(filePath);
}

void HTTPRes::file()
{
	std::cout << "ES FICHERO" << std::endl;

	std::string filePath = _config->getRoot(_location) + _request.getHeader("Path");

	std::cout << _location << std::endl;
	std::cout << _config->getRoot(_location) << std::endl;
	std::cout << filePath << std::endl;


	createContent(filePath);

}

void HTTPRes::methodGet()
{
	std::string path = _request.getHeader("Path");

	std::cout << _location << std::endl;

	std::cout << MAGENTA;
	if (isFile(path))
		file();
	else
		folder();
		
	std::cout << RESET;

	//=========================================================================

	// if (path == "/")
	// {
	// 	// TODO
	// 	// si no hay index.html, enviar 403 Forbidden
	// 	std::cout << ORANGE << "RAIZ\n";
	// 	rootPath();
	// }
	// // si tiene extension.
	// else if (isFile(path))
	// 	rootOtherFiles();
	// else if (!isFile(path))
	// {
	// 	std::cout << ORANGE << "Es una carpeta\n"
	// 			  << RESET;

	// 	if (isUrlAllowed(path))
	// 	{
	// 		// AQUI LOS LOCATION PERMITIDOS
	// 		std::cout << ORANGE << "PERMITIDO\n"
	// 				  << RESET;
	// 	}
	// 	else
	// 	{
	// 		// AQUI error 404
	// 		std::cout << RED << "NO PERMITIDO\n"
	// 				  << RESET;
	// 		error404();
	// 	}
	// }
	// // else
	// // {
	// // 	std::cout << ORANGE << "otros dentro de raiz\n"
	// // 			  << RESET;
	// // 	_header.addField("Extenxion", "Otros");
	// // }
}

void HTTPRes::exploreFiles()
{
	_header.addOne(_request.getHeader("Version"), "200 OK");
	_content = "FILE EXPLORER ";
}

void HTTPRes::methodPost()
{
	std::cout << "==========POST==========\n";
	std::string header;
	_content = _request.getHeader("Content");

	// std::string header = _request.getHeader("Method") + " /api/users";
	// std::string header = _request.getHeader("Method") + " /api/auth/register";
	// if (_request.getHeader("Referer") != "")
	// {
	// 	std::string newPath = +" /api/" + extractEndpoint(_request.getHeader("Referer"));
	// 	header = _request.getHeader("Method") + newPath;
	// }
	// else
	header = _request.getHeader("Method") + " " + _request.getHeader("Path");

	_header.addOne(header, _request.getHeader("Version"));

	_header.addField("Host", "localhost:8080");
	_header.addField("Content-Type", "application/json; charset=utf-8");
	_header.addField("Content-Length", std::to_string(_content.length()));
	_header.addField("Cookie", _request.getHeader("Cookie"));
	_header.addField("Date", DateField());
	_header.addField("42-Barcelona", "nmota-bu, vduchi");
}

void HTTPRes::methodDelete()
{
	std::cout << "==========DELETE==========\n";
}

void HTTPRes::methodErr()
{
	std::cout << "==========methodErr==========\n";
	_header.addOne(_request.getHeader("Version"), "405 Method Not Allowed");
	_content = readFile("./conf_web/error/basico/405.html");
}

std::string const HTTPRes::getHeader() const { return _header.getHeader(); }

std::string const HTTPRes::getContent() const { return _content; }

void HTTPRes::error404()
{
	// TODO
	_header.addOne(_request.getHeader("Version"), "404 Not Found");
	_content = readFile("./conf_web/error/basico/404.html");
	// poner el path en variable
	_config->setFirst(false);
	_config->setPrePath("./conf_web/error/404");
}
