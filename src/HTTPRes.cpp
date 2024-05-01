/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRes.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:54:23 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/01 18:39:14 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRes.hpp"

// bool HTTPRes::YesLocation()
// {

// for (std::map<std::string, std::map<std::string, std::string> >::const_iterator it = _locations.begin(); it != _locations.end(); ++it)
// 	{
// 		std::cout << GREEN;
// 			std::cout << it->first << std::endl;
// 			if(it->first == _request.getHeader("Path"))
// 				{
// 					std::cout<< ORANGE << "TOMA\n";

// 					return true;
// 				}
// 				else
// 				{
// 				_header.addOne(_request.getHeader("Version"), "404 Not Found");
// 					// TODO
// 					// enviar 404.html
// 					// poner los errores en map string dentro clase HTTPError
// 					return false;
// 				}

// 		std::cout << RESET;

// 	}
// 	return true;

// }

// bool HTTPRes::isUrlAllowed(const std::string& url, const std::string& method) const {
//     // Verificar si la URL coincide exactamente con el location "/"
//     if (url == "/") {
//         return true; // Permitir acceso a todos los archivos bajo el location "/"
//     }

//     // Obtener la configuración del location correspondiente a la URL
//     std::map<std::string, std::map<std::string, std::string> >::const_iterator it = _locations.find(url);
//     if (it != _locations.end()) {
//         // Verificar si el método está permitido para este location
//         std::map<std::string, std::string> locationConfig = it->second;
//         std::string allowedMethods = locationConfig["allow_methods"];

//         // Comprobar si el método de la solicitud está en la lista de métodos permitidos
//         if (allowedMethods.find(method) != std::string::npos) {
//             return true; // El método está permitido para esta URL
//         }
//     }
//     return false; // La URL no está configurada o el método no está permitido
// }

bool HTTPRes::isUrlAllowed(const std::string &url) const
{
	std::map<std::string, std::map<std::string, std::string> >::const_iterator it = _locations.find(url);
	if (it != _locations.end())
		return true;

	return false;
}

HTTPRes::HTTPRes(const HTTPRequest &request, const ServerConfig &config) : _request(request), _config(config)
{
	// TODO
	// [ Server Configuration ]
	_config.print();


	std::map<std::string, void (*)(HTTPRes &res)> function;

	function["GET"] = &methodGet;
	function["POST"] = &methodPost;
	function["DELETE"] = &methodDelete;

	std::map<std::string, void (*)(HTTPRes &res)>::iterator it = function.find(request.getHeader("Method"));
	if (it != function.end())
		it->second(*this);
	else
		methodErr(*this);
}

HTTPRes::~HTTPRes() {}

// void HTTPRes::rootPath(HTTPRes &res)
// {
// 	res._content = readFile("./dist/index.html");

	
// }

// void HTTPRes::rootOtherFiles()
// {

// }


void HTTPRes::methodGet(HTTPRes &res)
{



	std::string path = res._request.getHeader("Path");
	std::string extension = getExtension(path);



	std::cout << MAGENTA << " [HTTPRes::methodGet ]";
	std::cout << "Path: " << path << " extension: " << extension << std::endl;
	std::cout  << res.isUrlAllowed(path) << std::endl;

	std::cout << RESET;

		// if (res.isUrlAllowed(path) && res._init == false)
	// {
	// 	// res._init = true;
	// }
		if (path == "/")
			res._content = readFile("./dist/index.html");
		else
			// res._content = readFile("./dist" + path);
			res._content = readFile(res._config.getRootDirectory() + path);
	// }
	// else
	// {
	// 	res._header.addField("Content-Type", "text/html; charset=UTF-8");		
	// 	res._content = readFile("./conf_web/error/404/404.html");
	// }

	// comprueba si el fichero a pasar tiene datos	
	if (!res._content.empty())
		res._header.addOne(res._request.getHeader("Version"), "200 OK");
	else
		res._header.addOne(res._request.getHeader("Version"), "404 Not Found");

	// esto es solo para la raiz
	if (path == "/")
		res._header.addField("Content-Type", "text/html; charset=UTF-8");
	else
		// res._header.addField("Content-Type", mime.getContentType(extension));
		res._header.addField("Content-Type", res._config.getContentType(extension));
		

	res._header.addField("Content-Length", std::to_string(res._content.length()));
	res._header.addField("Cookie", res._request.getHeader("Cookie"));
	res._header.addField("Date", DateField());
	res._header.addField("ETag", generateETag(res._content));
	res._header.addField("Server", "ACME co.");
	res._header.addField("42-Barcelona", "nmota-bu, vduchi");
}

// void HTTPRes::methodGet(HTTPRes &res)
// {

// 	std::string path = res._request.getHeader("Path");
// 	std::string extension = getExtension(path);

// 	std::cout << RED << res.isUrlAllowed(path) << std::endl;

// 		if (path == "/")
// 			res._content = readFile("./dist/index.html");
// 		else
// 			// res._content = readFile("./dist" + path);
// 			res._content = readFile(res._config.getRootDirectory() + path);

// 	// comprueba si el fichero a pasar tiene datos	
// 	if (!res._content.empty())
// 		res._header.addOne(res._request.getHeader("Version"), "200 OK");
// 	else
// 		res._header.addOne(res._request.getHeader("Version"), "404 Not Found");

// 	// esto es solo para la raiz
// 	if (path == "/")
// 		res._header.addField("Content-Type", "text/html; charset=UTF-8");
// 	else
// 		// res._header.addField("Content-Type", mime.getContentType(extension));
// 		res._header.addField("Content-Type", res._config.getContentType(extension));
		

// 	res._header.addField("Content-Length", std::to_string(res._content.length()));
// 	res._header.addField("Cookie", res._request.getHeader("Cookie"));
// 	res._header.addField("Date", DateField());
// 	res._header.addField("ETag", generateETag(res._content));
// 	res._header.addField("Server", "ACME co.");
// 	res._header.addField("42-Barcelona", "nmota-bu, vduchi");
// }


void HTTPRes::methodPost(HTTPRes &res)
{
	std::cout << "==========POST==========\n";
	std::string header;
	res._content = res._request.getHeader("Content");

	// std::string header = res._request.getHeader("Method") + " /api/users";
	// std::string header = res._request.getHeader("Method") + " /api/auth/register";
	// if (res._request.getHeader("Referer") != "")
	// {
	// 	std::string newPath = +" /api/" + extractEndpoint(res._request.getHeader("Referer"));
	// 	header = res._request.getHeader("Method") + newPath;
	// }
	// else
	header = res._request.getHeader("Method") + " " + res._request.getHeader("Path");

	res._header.addOne(header, res._request.getHeader("Version"));

	res._header.addField("Host", "localhost:8080");
	res._header.addField("Content-Type", "application/json; charset=utf-8");
	res._header.addField("Content-Length", std::to_string(res._content.length()));
	res._header.addField("Cookie", res._request.getHeader("Cookie"));
	res._header.addField("Date", DateField());
	res._header.addField("42-Barcelona", "nmota-bu, vduchi");
}

void HTTPRes::methodDelete(HTTPRes &res)
{
	(void)res;
	std::cout << "==========DELTE==========\n";
}

void HTTPRes::methodErr(HTTPRes &res)
{
	std::cout << "==========ERROR==========\n";
	res._header.addOne(res._request.getHeader("Version"), "405 Method Not Allowed");
	res._header.addField("Date", DateField());
	res._header.addField("42-Barcelona", "nmota-bu, vduchi");
}

std::string const HTTPRes::getHeader() const { return _header.getHeader(); }

std::string const HTTPRes::getContent() const { return _content; }
