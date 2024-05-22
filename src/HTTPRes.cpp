/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRes.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:54:23 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/22 11:49:46 by nmota-bu         ###   ########.fr       */
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

// Función para quitar la cadena location del inicio de la cadena path
// std::string removeLocationFromPath(const std::string &location, const std::string &path)
// {
// 	if (path.find(location) == 0)
// 	{										   // Verifica si path comienza con location
// 		return path.substr(location.length()); // Retorna path sin la parte de location
// 	}
// 	return path; // Si path no comienza con location, retorna path sin cambios
// }

void removeLocationFromPath(const std::string &location, std::string &path)
{
	if (path.find(location) == 0)
		path = path.substr(location.length()); // Modifica path eliminando la parte de location
}

HTTPRes::HTTPRes(const HTTPRequest &request, ServerConfig *config) : _request(request), _config(config)
{

	std::string method = _request.getHeader("Method");
	std::string path = _request.getHeader("Path");
	std::string referer = _request.getHeader("Referer");
	std::string referer_locat = removeBeforeNumber(referer, "8080");

	// std::string remove_FileName = removeFileName(referer_locat);

	std::cout << CYAN;
	std::cout << "path: " << path << std::endl;
	std::cout << "referer: " << referer << std::endl;
	std::cout << "referer_locat: " << referer_locat << std::endl;
	// std::cout << "remove_FileName: " << remove_FileName << std::endl;
	std::cout << RESET;

	// referer.empty() ? std::cout << "referer: NO " << std::endl : std::cout << "referer: SI " << std::endl;

	LocationResult location;
	// std::string after = path;
	if (referer.empty()) // SIN refer
	{
		std::cout << "referer: NO " << std::endl;
		location = _config->getLocationConfig(path);
	}
	else
	{
		std::cout << "referer: SI " << std::endl;
		location = _config->getLocationConfig(removeFileName(referer_locat));
		removeLocationFromPath(location.name, path);
	}

	std::cout << "location.name: " << location.name << std::endl;
	std::cout << "path after: " << path << std::endl;

	//=========================================================================

	//=========================================================================

	// si tiene referer cabiar path por referer con un if y no enviar refer
	// pasar tokemo y el path sin location/
	// _locationConf.init(_config->getLocationConfig(path), path, referer_locat);
	_locationConf.init(location, path);

	std::cout << ORANGE;
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
		{
			std::cout << "realPath: " << _locationConf.realPath() << std::endl;

			if (isFile(_locationConf.realPath()))
				createContent(_locationConf.realPath(), true);
			else
				error403();
		}
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
	else
		file ? error404() : error403();
}

// 403 Forbidden cuando es web y va a carpta

// void HTTPRes::methodGet()
// {
// 	std::string path = _request.getHeader("Path");

// 	// TODO
// 	std::cout << MAGENTA;

// 	isFile(path) ? file() : folder();

// 	std::cout << RESET;
// }

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
		createContent(realpath, false);
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
