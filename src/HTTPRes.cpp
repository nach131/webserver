/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRes.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:54:23 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/20 13:12:51 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "HTTPRes.hpp"

void HTTPRes::last()
{
	// _header.addField("Cookie", _request.getHeader("Cookie"));

	_header.addField("Content-Length", std::to_string(_content.length()));
	_header.addField("ETag", generateETag(_content));

	_header.addField("Date", DateField());
	_header.addField("Server", "ACME co.");
	_header.addField("42-Barcelona", "nmota-bu, vduchi");
}

#include <map>

std::string removeBeforeNumber(const std::string &url, const std::string &host)
{

	std::cout << "HOST: " << host << std::endl;

	std::string result = url;
	// std::size_t pos = result.find("8080"); // Buscar el número "8080"
	std::size_t pos = result.find("localhost:8080"); // Buscar el número "8080"
	// std::size_t pos = result.find(toma); // Buscar el número "8080"

	if (pos != std::string::npos)
	{
		// Encontrar la siguiente barra después del número
		pos = result.find("/", pos);
		if (pos != std::string::npos)
		{
			// Eliminar todo lo anterior e incluyendo la barra
			// result = result.substr(pos + 1);
			result = result.substr(pos);
		}
		else
		{
			// Si no hay barra después del número, eliminar todo lo anterior e incluyendo el número
			result = "";
		}
	}

	return result;
}

HTTPRes::HTTPRes(const HTTPRequest &request, ServerConfig *config) : _request(request), _config(config)
{

	std::string method = _request.getHeader("Method");
	std::string path = _request.getHeader("Path");
	std::string referer = _request.getHeader("Referer");
	std::string host = _request.getHeader("Host");

	std::cout << "referer: " << referer << std::endl;
	std::cout << "host: " << host << std::endl;
	// http://localhost:8080/
	std::string remove = removeBeforeNumber(referer, host);

	_locationConf.init(_config->getLocationConfig(path), path, referer);

	std::cout << ORANGE;
	std::cout << "removeBeforeNumber: " << remove << std::endl;
	_locationConf.print();
	std::cout << RESET;

	//=========================================================================

	if (!_locationConf.methodAllowed(method))
		methodErr();
	else if (_locationConf.autoIndexOn())
	{
		std::cout << "EXPLORER\n";
		if (method == "GET")
			exploreFiles();
		else if (method == "POST")
			std::cout << "EXPLORE POST\n";
		else if (method == "DELETE")
			std::cout << "EXPLORE DELETE\n";
	}
	else
	{
		std::cout << "WEB\n";
		if (method == "GET")
			// std::cout << "WEB GET\n";
			createContent(_locationConf.realPath(), true);
		else if (method == "POST")
			std::cout << "WEB POST\n";
		else if (method == "DELETE")
			std::cout << "WEB DELETE\n";
	}

	//=========================================================================

	last();
}

HTTPRes::~HTTPRes() {}

//=========================================================================

std::string HTTPRes::execPython(const std::string &filePath)
{
	std::string result;
	FILE *pipe = popen(("python3 " + filePath).c_str(), "r");

	if (!pipe)
	{
		std::cerr << "Error: Failed to open pipe for Python script execution." << std::endl;
		return result;
	}

	char buffer[MAX_MSG_SIZE];
	while (fgets(buffer, sizeof(buffer), pipe) != NULL)
		result += buffer;

	int returnCode = pclose(pipe);
	if (returnCode != 0)
		std::cerr << "Error: Python script execution failed with return code " << returnCode << "." << std::endl;

	// std::cout << result << std::endl;

	return result;
}

void HTTPRes::createContent(std::string filePath, bool file)
{
	std::string extension = getExtension(filePath);

	if (extension == "py")
		_content = execPython(filePath);
	else
		_content = readFile(filePath);
	// comprueba si el fichero a pasar tiene datos
	if (!_content.empty())
	{
		_header.addOne(_request.getHeader("Version"), "200 OK");
		if (extension == "py")
			_header.addField("Content-Type", "text/html");
		else
			_header.addField("Content-Type", _config->getContentType(extension));
	}
	else // "false" = dentro de carpeta y no encuentra index.html
		file ? error404() : error403();

	// TODO
	// std::cout << "header: " << _header.getHeader() << std::endl;
	// std::cout << "content: " << _content << std::endl;
}

void HTTPRes::folder()
{
	// std::cout << "CARPETA" << std::endl;

	// std::string index = _config->getIndex(_location);
	// bool autoindex = _config->isLocationOn(_location);
	// std::string filePath;

	// if (index.empty())
	// {
	// 	if (_location == "/")
	// 		filePath = _config->getRoot(_location) + _request.getHeader("Path") + "index.html";
	// 	else
	// 		filePath = _config->getRoot(_location) + _request.getHeader("Path") + "/index.html";
	// }
	// else
	// 	filePath = _config->getRoot(_location) + _request.getHeader("Path") + "/" + index;

	// // TODO
	// std::cout << " _location: " << _location << std::endl;
	// std::cout << " autoindex: " << autoindex << std::endl;
	// std::cout << " index: " << index << std::endl;
	// std::cout << " filePath: " << filePath << std::endl;

	// createContent(filePath, false);
}

void HTTPRes::file()
{
	// 		std::cout << "ES FICHERO" << std::endl;
	// 		std::string filePath;

	// 		if (isMainRoot(_location))
	// 			filePath = _config->getRoot("/") + _request.getHeader("Path");
	// 		else
	// 			filePath = _config->getRoot(_location) + _request.getHeader("Path");

	// 		// TODO
	// 		std::cout << "_location: " << _location << std::endl;
	// 		std::cout << "root: " << _config->getRoot(_location) << std::endl;
	// 		std::cout << "filePath: " << filePath << std::endl;

	// 		createContent(filePath, true);
}

void HTTPRes::methodGet()
{
	std::string path = _request.getHeader("Path");

	// TODO
	std::cout << MAGENTA;

	isFile(path) ? file() : folder();

	std::cout << RESET;
}

std::string pyExplorer(const std::string &py, const std::string &dirPath, const std::string &root_location)
{
	std::string result;

	std::cout << "PY EXPLORER\n";
	std::cout << "dirPath: " << dirPath << std::endl;

	FILE *pipe = popen(("python3 " + py + " " + dirPath + " " + root_location).c_str(), "r");
	if (!pipe)
	{
		std::cerr << "Error: Failed to open pipe for Python script execution." << std::endl;
		return result;
	}

	char buffer[MAX_MSG_SIZE];
	while (fgets(buffer, sizeof(buffer), pipe) != NULL)
		result += buffer;

	int returnCode = pclose(pipe);
	if (returnCode != 0)
		std::cerr << "Error: Python script execution failed with return code " << returnCode << "." << std::endl;

	return result;
}

void HTTPRes::exploreFiles()
{
	std::string realpath = _locationConf.realPath();

	if (!isFile(realpath))
	{
		std::cout << "CARPETA\n";

		std::string py = "./cgi_bin/explorer.py";
		_header.addOne(_request.getHeader("Version"), "200 OK");
		_header.addField("Content-Type", "text/html");

		_content = pyExplorer(py, realpath, _locationConf.getRoot());
	}
	else
	{
		std::cout << "FILE\n";
		createContent(realpath, true);
	}
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

std::string const HTTPRes::getHeader() const { return _header.getHeader(); }

std::string const HTTPRes::getContent() const { return _content; }
