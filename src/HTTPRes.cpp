/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRes.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:54:23 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/25 13:52:41 by vduchi           ###   ########.fr       */
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
	std::string referer_location = removeBeforeNumber(referer, "8080");

	std::cout << CYAN;
	std::cout << "path: " << path << std::endl;
	std::cout << "referer: " << referer << std::endl;
	std::cout << "referer_location: " << referer_location << std::endl;
	std::cout << RESET;

	LocationResult location = compareLocationResults(_config->getLocationConfig(path), _config->getLocationConfig(referer_location));

	std::cout << "location.name: " << location.name << std::endl;

	//=========================================================================

	//=========================================================================

	_locationConf.init(location, path, referer_location);

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
	std::string el;
	std::string result;

	std::cout << "PY EXPLORER\n";
	std::cout << "dirPath: " << dirPath << std::endl;

	// FILE *pipe = popen(("python3 " + py + " " + dirPath + " " + root_location).c_str(), "r");
	// if (!pipe)
	// {
	// 	std::cerr << "Error: Failed to open pipe for Python script execution." << std::endl;
	// 	return result;
	// }
	std::system(("python3 " + py + " " + dirPath + " " + root_location + " > /Users/nmota-bu/Desktop/test.txt").c_str());
	std::stringstream ss;
	ss << std::ifstream("/Users/nmota-bu/Desktop/test.txt").rdbuf();
	result = ss.str();
	std::cout << RED << "TOMATE\n"
						<< result << RESET << std::endl;
	// while (in >> el)
	// 	result.append(el);

	// char buffer[MAX_MSG_SIZE];
	// while (fgets(buffer, sizeof(buffer), pipe) != NULL)
	// 	result += buffer;

	// int returnCode = pclose(pipe);
	// if (returnCode != 0)
	// 	std::cerr << "Error: Python script execution failed with return code " << returnCode << "." << std::endl;

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

		std::string root = _locationConf.getRoot().empty() ? _locationConf.getAlias() : _locationConf.getRoot();

		// _content = pyExplorer(py, realpath, _locationConf.getRoot());
		_content = pyExplorer(py, realpath, root);
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
