/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRes.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:54:23 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/26 19:09:56 by nmota-bu         ###   ########.fr       */
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
		{
			std::cout << "POST\n";

			// std::cout << "realPath: " << _locationConf.realPath() << std::endl;
			// realPath: .//cgi_bin/register.py SOLUCIONAR
			// std::cout << "BUFFER: \n"
			// 		  << _request.getBuffer() << std::endl;

			_header.addOne(_request.getHeader("Version"), "200 OK");
			_header.addField("Content-Type", "text/html");

			_content = pyExplorer("./cgi_bin/register.py", _request.getHeader("Content"), "");
		}
		else if (method == "DELETE")
		{
			std::cout << "WEB DELETE\n";
			methodDelete();
		}
	}

	//=========================================================================

	last();
}

HTTPRes::~HTTPRes() {}

//=========================================================================

std::string HTTPRes::execPython(const std::string &filePath, const std::string &realBuffer)
{
	std::string result;
	std::cout << "execPython\n";

	FILE *pipe = popen(("python3 " + filePath + " " + realBuffer).c_str(), "r");

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

//=========================================================================

void HTTPRes::createContent(std::string filePath, bool file)
{
	std::string extension = getExtension(filePath);

	// TODO comprobar que el py es de la lista que se pueden ejecutat
	// hacer lista en config file
	if (extension == "py")
	{
		std::cout << "execPython:" << std::endl;
		_content = execPython(filePath);
	}
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

std::string HTTPRes::pyExplorer(const std::string &py, const std::string &dirPath, const std::string &root_location)
{
	std::string result;

	std::cout << "pyExplorer\n";
	std::cout << " dirPath: " << dirPath << std::endl;
	// Verificar la existencia del script de Python
	// if (!std::ifstream(py).good())
	// 	throw std::runtime_error("Error: No se encontró el archivo del script de Python: " + py);

	// // Verificar la existencia del directorio de destino
	// if (!std::ifstream(dirPath).good())
	// 	throw std::runtime_error("Error: No se encontró el directorio especificado: " + dirPath);

	// Construir el comando del sistema
	std::string command = "python3 " + py + " " + dirPath + " " + root_location + " > ./conf_web/.tmp";
	int returnCode = std::system(command.c_str());

	// Verificar el éxito del comando del sistema
	if (returnCode != 0)
		throw std::system_error(returnCode, std::generic_category(), "Error al ejecutar el comando del sistema");

	// Leer el archivo temporal
	std::ifstream tempFile("./conf_web/.tmp");
	if (!tempFile.good())
		throw std::runtime_error("Error: No se pudo abrir el archivo temporal para leer");

	std::stringstream ss;
	ss << tempFile.rdbuf();
	result = ss.str();

	// Limpiar el archivo temporal
	// if (std::remove("./conf_web/.tmp") != 0)
	// 	std::cerr << "Advertencia: No se pudo eliminar el archivo temporal" << std::endl;

	return result;
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

		_content = pyExplorer(py, realPath, root);
	}
	else
	{
		std::cout << "FILE\n";
		createContent(realPath, false);
	}
}

void HTTPRes::methodPost()
{
	std::cout << "==========POST==========\n";
	std::string header;
	_content = _request.getHeader("Content");

	// PARA API
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

	_content = pyExplorer(py, realPath, realFileName);
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
