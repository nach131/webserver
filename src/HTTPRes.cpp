/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRes.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:54:23 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/19 19:38:26 by nmota-bu         ###   ########.fr       */
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

HTTPRes::HTTPRes(const HTTPRequest &request, ServerConfig *config) : _request(request), _config(config)
{

	std::string method = _request.getHeader("Method");
	std::string path = _request.getHeader("Path");

	_locationConf.init(_config->getLocationConfig(path), path);

	std::cout << ORANGE;
	_locationConf.print();
	// std::cout << "locationName: " << _locationConf.getName()<< std::endl;
	// std::cout << "realPath: " << _locationConf.realPath()<< std::endl;
	std::cout << RESET;
	// std::cout << "autoIndexOn: " << _locationConf.autoIndexOn()<< std::endl;
	// std::cout << "methodAllowed: " << _locationConf.methodAllowed(method)<< std::endl;
	// std::cout << "realPath: " << _locationConf.realPath()<< std::endl;

	//=========================================================================

	if (!_locationConf.methodAllowed(method))
		methodErr();
	else if (_locationConf.autoIndexOn())
	{
		exploreFiles();
	}

	else
	{
		std::cout << "WEB\n";
	}

	// else
	// {
	// 	// if(_locationConf.autoIndexOn() && method == "GET" )
	// 	// std::cout << " index On\n";
	// 	// 		// exploreFiles();
	// 	// else if (!_locationConf.autoIndexOn() && method == "GET")
	// 	// std::cout << " index Off\n";
	// // 	methodGet();

	// }

	//=========================================================================
	// _location = _request.getLocation(); // getLocation en config y devuelve un map
	// bool  fav = _location == "/favicon.ico";

	// std::cout << "_location: " << _location << std::endl;
	// std::cout << "is file: " << isFile(_request.getHeader("Path")) << std::endl;

	// if (method == "GET" && _config->isLocationAllowed(_location) && _config->isMethodAllowed(_location, "GET") && _config->isLocationOn(_location))
	// {
	// 	exploreFiles();
	// }
	// else if ((method == "GET" && _config->isLocationAllowed(_location) && _config->isMethodAllowed(_location, "GET")) || isFile(_request.getHeader("Path")))
	// 	methodGet();
	// else if (method == "POST" && _config->isMethodAllowed(_location, "POST"))
	// 	methodPost();
	// else if (method == "DELETE" && _config->isMethodAllowed(_location, "DELETE"))
	// 	methodDelete();
	// else
	// 	methodErr();

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
	// 	std::cout << "ES FICHERO" << std::endl;
	// 	std::string filePath;

	// 	if (isMainRoot(_location))
	// 		filePath = _config->getRoot("/") + _request.getHeader("Path");
	// 	else
	// 		filePath = _config->getRoot(_location) + _request.getHeader("Path");

	// 	// TODO
	// 	std::cout << "_location: " << _location << std::endl;
	// 	std::cout << "root: " << _config->getRoot(_location) << std::endl;
	// 	std::cout << "filePath: " << filePath << std::endl;

	// 	createContent(filePath, true);
}

void HTTPRes::methodGet()
{
	std::string path = _request.getHeader("Path");

	// TODO
	std::cout << MAGENTA;

	isFile(path) ? file() : folder();

	std::cout << RESET;
}

std::string toma(const std::string &py, const std::string &dirPath, const std::string &locate)
{
	std::string result;

	FILE *pipe = popen(("python3 " + py + " " + dirPath + " " + locate).c_str(), "r");
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

	std::cout << result << std::endl;

	return result;
}

void HTTPRes::exploreFiles()
{
	// TODO
	std::cout << "FILES EXPLORER" << std::endl;

	std::string realpath = _locationConf.realPath();
	std::string rootPath = _locationConf.getRoot();

	if (!isFile(realpath))
	{
		std::cout << "CARPETA\n";

		std::string py = "./cgi_bin/explorer.py";
		_header.addOne(_request.getHeader("Version"), "200 OK");
		_header.addField("Content-Type", "text/html");

		_content = toma(py, realpath, _locationConf.getName());
	}
	else
	{
		std::cout << "ES FILE\n";
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
