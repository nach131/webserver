/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRes.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:54:23 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/30 16:22:15 by nmota-bu         ###   ########.fr       */
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

bool HTTPRes::isUrlAllowed(const std::string& url, const std::string& method) const {
    // Verificar si la URL coincide exactamente con el location "/"
    if (url == "/") {
        return true; // Permitir acceso a todos los archivos bajo el location "/"
    }

    // Obtener la configuración del location correspondiente a la URL
    std::map<std::string, std::map<std::string, std::string> >::const_iterator it = _locations.find(url);
    if (it != _locations.end()) {
        // Verificar si el método está permitido para este location
        std::map<std::string, std::string> locationConfig = it->second;
        std::string allowedMethods = locationConfig["allow_methods"];

        // Comprobar si el método de la solicitud está en la lista de métodos permitidos
        if (allowedMethods.find(method) != std::string::npos) {
            return true; // El método está permitido para esta URL
        }
    }
    return false; // La URL no está configurada o el método no está permitido
}



HTTPRes::HTTPRes(const HTTPRequest &request, const ServerConfig &config) : _request(request), _config(config)
{

	// config.print();
	_config.print();
	_locations = config.getLocation();
	
	// if(!isUrlAllowed(request.getHeader("Path"), request.getHeader("Method")))
	// {
	// 	// return;
	// 	std::cout << ORANGE << "NO PERMITIDA\n";
	// }
	// else
	// {
	// 	std::cout << ORANGE << "PERMITIDA\n";
	// }
	
		
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

void HTTPRes::methodGet(HTTPRes &res)
{
	// MIMETypeReader mime("./conf_web/mime.type");
	
	std::string path = res._request.getHeader("Path");
	std::string extension = getExtension(path);


	std::string contentType = res._config.getContentType(extension) ;
    std::cout <<ORANGE << "Content type for "<< extension<<" files: " << contentType << RESET<<std::endl;
	
	if (path == "/")
		res._content = readFile("./dist/index.html");
	else
		res._content = readFile("./dist" + path);

	if (!res._content.empty())
		res._header.addOne(res._request.getHeader("Version"), "200 OK");
	else
		res._header.addOne(res._request.getHeader("Version"), "404 Not Found");

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
