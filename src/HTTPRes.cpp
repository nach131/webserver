/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRes.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:54:23 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/28 10:33:03 by nmota-bu         ###   ########.fr       */
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

HTTPRes::HTTPRes(const HTTPRequest &request, ServerConfig *config, const bool &ref) : _request(request), _config(config), _ref(ref)
{
	std::string method = _request.getHeader("Method");
	std::string path = _request.getHeader("Path");
	std::string referer = _request.getHeader("Referer");
	std::string referer_location = removeBeforeNumber(referer, "8080");

	std::cout << CYAN;
	std::cout << "_ref bool: " << _ref << std::endl;
	std::cout << "path: " << path << std::endl;
	std::cout << "referer: " << referer << std::endl;
	std::cout << "referer_location: " << referer_location << std::endl;
	std::cout << RESET;

	LocationResult location = compareLocationResults(_config->getLocationConfig(path), _config->getLocationConfig(referer_location));

	std::cout << "location.name: " << location.name << std::endl;

	//=========================================================================

	//=========================================================================

	_locationConf.init(location, path, referer_location, _ref);

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
		{
			std::cout << "EXPLORE POST\n";
			methodPost(true);
		}
		else if (method == "DELETE")
			methodDelete();
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
		{
			std::cout << "==========POST==========\n";
			methodPost(false);
		}
		else if (method == "DELETE")
			methodDelete();
	}

	//=========================================================================

	last();
}

HTTPRes::~HTTPRes() {}

//=========================================================================

void HTTPRes::createContent(std::string filePath, bool file)
{
	std::string extension = getExtension(filePath);

	// TODO comprobar que el py es de la lista que se pueden ejecutat
	// hacer lista en config file

	std::cout << RED << "extension: " << extension << RESET << std::endl;

	if (extension == "py")
	{
		std::cout << "execPython:" << std::endl;
		// TODO
		// _content = execPython(filePath);
	}
	else if (extension == "php")
	{
		// error501();
		_content = execPython("./cgi_bin/php.py");
	}
	else
		_content = readFile(filePath);
	// comprueba si el fichero a pasar tiene datos
	if (!_content.empty())
	{
		_header.addOne(_request.getHeader("Version"), "200 OK");
		if (extension == "py" || extension == "php")
			_header.addField("Content-Type", "text/html");
		else
			_header.addField("Content-Type", _config->getContentType(extension));
	}
	else
		file ? error404() : error403();
}

void HTTPRes::exploreFiles()
{
	std::string realPath = _locationConf.realPath();

	if (!isFile(realPath))
	{
		std::cout << "CARPETA\n";

		std::string py = "./cgi_bin/explorer.py";
		_header.addOne(_request.getHeader("Version"), "200 OK");
		_header.addField("Content-Type", "text/html");

		std::string root = _locationConf.getRoot().empty() ? _locationConf.getAlias() : _locationConf.getRoot();

		_content = execPython(py, realPath, root);
	}
	else
	{
		std::cout << "FILE\n";
		createContent(realPath, false);
	}
}

// TEXTO
// bool writeToFile(const std::string &filePath, const std::string &content)
// {
// 	std::ofstream outputFile(filePath.c_str());
// 	if (!outputFile.is_open())
// 	{
// 		std::cerr << "Error: Unable to open file: " << filePath << std::endl;
// 		return false;
// 	}

// 	outputFile << content;
// 	outputFile.close();
// 	return true;
// }

// bool writeToFile(const std::string &filePath, const std::string &content)
// {
// 	std::ofstream outputFile(filePath.c_str(), std::ios::binary);
// 	if (!outputFile.is_open())
// 	{
// 		std::cerr << "Error: Unable to open file: " << filePath << std::endl;
// 		return false;
// 	}

// 	// Escribir los bytes del contenido en el archivo
// 	outputFile.write(content.c_str(), content.size());

// 	outputFile.close();
// 	return true;
// }

std::string charToBits(char ch)
{
	std::string result;
	for (int i = 7; i >= 0; --i)
	{
		result += ((ch >> i) & 1) ? '1' : '0';
	}
	return result;
}

std::string stringToBits(const std::string &input)
{
	std::string result;
	for (size_t i = 0; i < input.size(); ++i)
	{
		result += charToBits(input[i]);
	}
	return result;
}

bool writeToFile(const std::string &filePath, const std::string &content)
{
	std::ofstream outputFile(filePath.c_str(), std::ios::binary);
	if (!outputFile.is_open())
	{
		std::cerr << "Error: Unable to open file: " << filePath << std::endl;
		return false;
	}

	// // Convertir el contenido a bits
	// std::string contentBits = stringToBits(content);

	// // Escribir los bits en el archivo
	// outputFile.write(contentBits.c_str(), contentBits.size());

	outputFile.write(content.c_str(), content.size());

	outputFile.close();
	return true;
}

void HTTPRes::methodPost(const bool &autoindex)
{
	std::string realPath = _locationConf.realPath();

	std::cout << "getFileName: " << _request.getFileName() << std::endl;

	if (!directoryExists(realPath))
	{
		std::cout << " crea: " << realPath << std::endl;

		createDirectory(realPath);
	}

	if (!autoindex)
	{
		// _header.addOne(_request.getHeader("Version"), "200 OK");
		// _header.addField("Content-Type", "text/html");

		// _content = execPython("." + path, "{\"username\":\"nombre\",\"password\":\"121\",\"email\":\"swsw@sw.ws\"}");
		// _content = execPython("." + path, _request.getHeader("Content"));
		// _content = execPython("/Users/nmota-bu/Desktop/webserver/cgi_bin/register.py", _request.getHeader("Content"));

		std::cout << RED << _content << RESET << std::endl;
	}
	else
	{
		std::cout << " EXPLORE POST\n";

		if (isText(_request.getHeader("Content-Type")))
			writeToFile(realPath + "/text.txt", _request.getHeader("Content"));

		// std::cout << "content: " << _request.getHeader("Content") << std::endl;
	}
}

// void HTTPRes::methodPost()
// {
// 	std::cout << "==========POST==========\n";
// 	std::string header;
// 	_content = _request.getHeader("Content");

// 	// PARA API OLD POST
// 	// std::string header = _request.getHeader("Method") + " /api/users";
// 	// std::string header = _request.getHeader("Method") + " /api/auth/register";
// 	// if (_request.getHeader("Referer") != "")
// 	// {
// 	// 	std::string newPath = +" /api/" + extractEndpoint(_request.getHeader("Referer"));
// 	// 	header = _request.getHeader("Method") + newPath;
// 	// }
// 	// else
// 	header = _request.getHeader("Method") + " " + _request.getHeader("Path");

// 	_header.addOne(header, _request.getHeader("Version"));

// 	_header.addField("Host", "localhost:8080");
// 	_header.addField("Content-Type", "application/json; charset=utf-8");
// 	_header.addField("Content-Length", std::to_string(_content.length()));
// 	_header.addField("Cookie", _request.getHeader("Cookie"));
// 	_header.addField("Date", DateField());
// 	_header.addField("42-Barcelona", "nmota-bu, vduchi");
// }

void HTTPRes::methodDelete()
{
	std::cout << "==========DELETE==========\n";

	std::string realPath = _locationConf.realPath();
	std::string remove = _locationConf.getRoot().empty() ? _locationConf.getAlias() : _locationConf.getRoot();

	std::string FileName = removeSubstring(realPath, remove);
	std::string realFileName = getFileName(FileName);

	realPath = "./upload" + FileName;

	std::cout << "FileName: " << FileName << std::endl;
	std::cout << "realPath: " << realPath << std::endl;
	std::cout << "Real File name: " << getFileName(FileName) << std::endl;

	std::string py = "./cgi_bin/delete.py";
	_header.addOne(_request.getHeader("Version"), "200 OK");
	_header.addField("Content-Type", "text/html");

	_content = execPython(py, realPath, realFileName);
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

// void HTTPRes::error501()
// {
// 	_header.addOne(_request.getHeader("Version"), "501 Forbidden");
// 	_content = readFile("./conf_web/error/basico/501.html");
// }

std::string const HTTPRes::getHeader() const { return _header.getHeader(); }

std::string const HTTPRes::getContent() const { return _content; }
