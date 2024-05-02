/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRes.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:54:23 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/02 17:39:47 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRes.hpp"

bool HTTPRes::isUrlAllowed(const std::string &url) const
{
	std::map<std::string, std::map<std::string, std::string> > location = _config.getLocation();

	std::map<std::string, std::map<std::string, std::string> >::const_iterator it = location.find(url);
	if (it != location.end())
		return true;
	return false;
}

void HTTPRes::last()
{
	_header.addField("Cookie", _request.getHeader("Cookie"));

	_header.addField("Content-Length", std::to_string(_content.length()));
	_header.addField("ETag", generateETag(_content));

	_header.addField("Date", DateField());
	_header.addField("Server", "ACME co.");
	_header.addField("42-Barcelona", "nmota-bu, vduchi");
}


HTTPRes::HTTPRes(const HTTPRequest &request, const ServerConfig &config) : _request(request), _config(config)
{
	// TODO
	// [ Server Configuration ]
	// _config.print();

	std::string path = request.getHeader("Path");

	std::string method = request.getHeader("Method");
	if (method == "GET")
	{
		if (path == "/")
		{
			// TODO
			// si no hay index.html, enviar 403 Forbidden
			std::cout << ORANGE << "RAIZ\n";
			rootPath();
		}
		// si tiene extension.
		else if (isFile(path))
		{
			std::cout << ORANGE << "Si tiene extension\n" << RESET;
			rootOtherFiles(); // comprobar que es true el bool -> dentro de la funcion poner el path de la variable antesde lapath config
		}
		// es una carpeta
		else if (!isFile(path))
		{
			std::cout << ORANGE << "Es una carpeta\n" << RESET;

			_header.addField("Extenxion", "No tiene");
			
			if (isUrlAllowed(path))
			{
				// AQUI LOS LOCATION PERMITIDOS
				std::cout << ORANGE << "PERMITIDO\n" << RESET;
			}
			else
			{	
				// AQUI error 404
			std::cout << RED << "NO PERMITIDO\n" << RESET;
			_header.addOne(_request.getHeader("Version"), "404 Not Found");
			_content = readFile("./conf_web/error/404/404.html");
			// poner el path en variable



			}
			
		}		
		else
		{
			std::cout << ORANGE << "otros dentro de raiz\n" << RESET;
			_header.addField("Extenxion", "Otros");
		}
	}
	else if (method == "POST")
	{
		methodPost();
	}
	else if (method == "DELETE")
	{
		void methodDelete();
	}
	else
	{
		void methodErr();
	}

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

	// _header.addField("Content-Length", std::to_string(_content.length()));
	// _header.addField("Cookie", _request.getHeader("Cookie"));
	// _header.addField("Date", DateField());
	// _header.addField("ETag", generateETag(_content));
	// _header.addField("Server", "ACME co.");
	// _header.addField("42-Barcelona", "nmota-bu, vduchi");
}

void HTTPRes::rootOtherFiles()
{
	std::string path = _request.getHeader("Path");
	std::string extension = getExtension(path);

	_content = readFile(_config.getRootDirectory() + path);

	// comprueba si el fichero a pasar tiene datos
	if (!_content.empty())
		_header.addOne(_request.getHeader("Version"), "200 OK");
	else
		_header.addOne(_request.getHeader("Version"), "404 Not Found");

	_header.addField("Content-Type", _config.getContentType(extension));

	// _header.addField("Content-Length", std::to_string(_content.length()));
	// _header.addField("Cookie", _request.getHeader("Cookie"));
	// _header.addField("ETag", generateETag(_content));
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
	std::cout << "==========DELTE==========\n";
}

void HTTPRes::methodErr()
{
	std::cout << "==========ERROR==========\n";
	_header.addOne(_request.getHeader("Version"), "405 Method Not Allowed");
	_header.addField("Date", DateField());
	_header.addField("42-Barcelona", "nmota-bu, vduchi");
}

std::string const HTTPRes::getHeader() const { return _header.getHeader(); }

std::string const HTTPRes::getContent() const { return _content; }
