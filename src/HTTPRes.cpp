/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRes.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:54:23 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/15 21:24:26 by nmota-bu         ###   ########.fr       */
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

//=========================================================================

void HTTPRes::createContent(std::string filePath, bool file)
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
	    file ? error404() : error403();
}

void HTTPRes::folder()
{
	std::cout << "CARPETA" << std::endl;

	std::string index = _config->getIndex(_location);
	std::string filePath;

	if (index.empty())
	{
		if (_location == "/")
			filePath = _config->getRoot(_location) + _request.getHeader("Path") + "index.html";
		else
			filePath = _config->getRoot(_location) + _request.getHeader("Path") + "/index.html";
	}
	else
		filePath = _config->getRoot(_location) + _request.getHeader("Path") + "/" + index;

	std::cout << _location << std::endl;
	std::cout << index << std::endl;
	std::cout << filePath << std::endl;

	createContent(filePath, false);
}

void HTTPRes::file()
{
	std::cout << "ES FICHERO" << std::endl;

	std::string filePath = _config->getRoot(_location) + _request.getHeader("Path");

	std::cout << _location << std::endl;
	std::cout << _config->getRoot(_location) << std::endl;
	std::cout << filePath << std::endl;

	createContent(filePath, true);
}

void HTTPRes::methodGet()
{
	std::string path = _request.getHeader("Path");

	std::cout << MAGENTA;

	isFile(path) ? file() : folder();
	// if (isFile(path))
	// 	file();
	// else
	// 	folder();

	std::cout << RESET;
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
	_header.addOne(_request.getHeader("Version"), "404 Not Found");
	_content = readFile("./conf_web/error/basico/404.html");
}

void HTTPRes::error403()
{
	_header.addOne(_request.getHeader("Version"), "403 Forbidden");
	_content = readFile("./conf_web/error/basico/403.html");
}

