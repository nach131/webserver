/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRes.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:54:23 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/06/02 13:42:08 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "HTTPRes.hpp"

void HTTPRes::last()
{
	_header.addField("Content-Length", std::to_string(_content.length()));
	_header.addField("ETag", generateETag(_content));

	_header.addField("Date", DateField());
	_header.addField("Server", "ACME co.");
	_header.addField("42-Barcelona", "nmota-bu, vduchi");
}

void removeLocationFromPath(const std::string &location, std::string &path)
{
	if (path.find(location) == 0)
		path = path.substr(location.length());
}

HTTPRes::HTTPRes(const HTTPRequest &request, ServerConfig *config, const bool &ref, bool &write, bool &send) : _request(request), _config(config), _ref(ref), _write(write), _send(send)
{
	std::string path = _request.getHeader("Path");
	std::string method = _request.getHeader("Method");
	std::string referer = _request.getHeader("Referer");

	std::string referer_location = removeBeforeNumber(referer, intToString(_config->getPort()));
	LocationResult location = compareLocationResults(_config->getLocationConfig(path), _config->getLocationConfig(referer_location));
	_locationConf.init(location, path, referer_location, _ref);
	if (!_locationConf.methodAllowed(method))
		error405();
	else if (_locationConf.autoIndexOn())
	{
		if (method == "GET")
			exploreFiles();
		else if (method == "POST")
			methodPost(true);
		else if (method == "DELETE")
			methodDelete(true);
	}
	else
	{
		if (method == "GET")
		{
			std::string filePath = _locationConf.realPath();
			if (!isFile(_request.getHeader("Path")) && !isFileExist(filePath))
				error403();
			else if (isFile(filePath))
				createContent(filePath);
		}
		else if (method == "POST")
			methodPost(false);
		else if (method == "DELETE")
			methodDelete(false);
	}
	last();
}

HTTPRes::~HTTPRes() {}

void HTTPRes::createContent(std::string filePath)
{

	std::string toma = _request.getHeader("Path");
	std::string cookie = _request.getHeader("Cookie");
	if (!cookie.empty())
	{
		cookie = removeSubstring(cookie, "Authenticator=");
		removeLastReturn(cookie);
	}
	std::string extension = getExtension(filePath);
	if (extension == "py")
	{
		if (_locationConf.realPath().find("photo.py") != std::string::npos)
			_content = execPython(_locationConf.realPath());
	}
	else if (extension == "php")
		_content = execPython("./cgi_bin/php.py");
	else if (isValidToken(cookie, 32) && _request.getHeader("Path") == "/web/logout.html")
	{
		_header.addOne(_request.getHeader("Version"), "200 OK");
		_header.addField("Content-Type", "text/html");
		std::string cookie = "Authenticator=0000";
		_header.addField("Set-Cookie", cookie);
		_content = readFile("./dist/web/index.html");
		Log::info(GET, "200 OK");
		return;
	}
	else if (_locationConf.getName() == "/web" && !cookie.empty() && isValidToken(cookie, 32) && extension == "html")
		_content = readFile("./dist/web/welcome.html");
	else
		_content = readFile(filePath);
	if (!_content.empty())
	{
		_header.addOne(_request.getHeader("Version"), "200 OK");
		if (extension == "py" || extension == "php")
			_header.addField("Content-Type", "text/html");
		else
			_header.addField("Content-Type", _config->getContentType(extension));
		Log::info(GET, "200 OK");
	}
	else
		error404();
}

void HTTPRes::exploreFiles()
{
	std::string realPath = _locationConf.realPath();
	if (!isFile(realPath))
	{
		if (directoryExists(realPath))
		{
			std::string py = "./cgi_bin/explorer.py";
			_header.addOne(_request.getHeader("Version"), "200 OK");
			_header.addField("Content-Type", "text/html");
			std::string root = _locationConf.getRoot().empty() ? _locationConf.getAlias() : _locationConf.getRoot();
			_content = execPython(py, realPath, root);
			Log::info(GET, "200 OK");
		}
		else
			error404();
	}
	else
		createContent(realPath);
}

void HTTPRes::methodPost(const bool &autoindex)
{
	std::string realPath = _locationConf.realPath();
	std::string fileName = _request.getFileName();
	if (!autoindex)
	{
		if (realPath.find("upload.py") != std::string::npos)
		{
			std::string ref = _locationConf.getRef();
			if (!directoryExists("./upload" + ref))
				createDirectory("./upload" + ref);
			if (_write)
			{
				if (!fileName.empty())
					execUpload("./upload" + ref + "/" + fileName);
				else
					Log::error(RESP, "Filename empty");
			}
		}
		else if (realPath.find("register.py") != std::string::npos)
		{
			_header.addOne(_request.getHeader("Version"), "301 Moved Permanently");
			_header.addField("Content-Type", "text/html");
			std::string res = execPython(realPath, _request.getHeader("Content"));
			res == "0\n" ? _header.addField("Location", "/web/register_ok.html") : _header.addField("Location", "/web/register_err.html");
			Log::info(POST, "301 Moved Permanently");
		}
		else if (realPath.find("login.py") != std::string::npos)
		{
			_header.addOne(_request.getHeader("Version"), "301 Moved Permanently");
			_header.addField("Content-Type", "text/html");
			std::string res = execPython(realPath, _request.getHeader("Content"));
			if (res == "0\n")
			{
				std::string cookie = "Authenticator=" + generateToken(32);
				cookie.append("; Path=/web");
				cookie.append("; Secure=false");
				_header.addField("Set-Cookie", cookie);
				_header.addField("Location", "/web/welcome.html");
			}
			else
				_header.addField("Location", "/web/login_err.html");
			Log::info(POST, "301 Moved Permanently");
		}
		else if (realPath.find("setcookie.py") != std::string::npos)
		{
			std::string cookie = execPython(realPath, _request.getHeader("Content"));
			_header.addOne(_request.getHeader("Version"), "204 No Content");
			_header.addField("Content-Type", "text/html");
			_header.addField("Set-Cookie", cookie);
			Log::info(POST, "204 No Content");
		}
	}
	else
	{
		if (_write)
		{
			std::string res;
			if (!directoryExists(realPath))
				createDirectory(realPath);
			if (!fileName.empty())
				execUpload(realPath + "/" + fileName);
			else
				Log::error(RESP, "Filename empty");
		}
	}
}

void HTTPRes::methodDelete(const bool &autoindex)
{
	std::string realPath = _locationConf.realPath();
	std::string remove = _locationConf.getRoot().empty() ? _locationConf.getAlias() : _locationConf.getRoot();
	std::string FileName = removeSubstring(realPath, remove);
	std::string realFileName = getFileName(FileName);
	std::string py = "./cgi_bin/delete.py";
	if (autoindex)
		_content = execPython(py, realPath, realFileName);
	else
	{
		realPath = "./upload" + FileName;
		_content = execPython(py, realPath, realFileName);
	}
	_header.addOne(_request.getHeader("Version"), "200 OK");
	_header.addField("Content-Type", "text/html");
	Log::error(DEL, "200 OK");
}

void HTTPRes::execUpload(const std::string &pathFile)
{
	std::string res = execPython("./cgi_bin/upload.py", pathFile);
	if (res == "0\n")
	{
		_header.addOne(_request.getHeader("Version"), "200 OK");
		_header.addField("Content-Type", "text/html");
		_content = readFile("./conf_web/error/basico/upload.html");
	}
	Log::info(POST, "200 OK");
	_write = false;
	_send = true;
}

void HTTPRes::error405()
{
	std::string custom = _config->getErrorPage(405);
	_header.addOne(_request.getHeader("Version"), "405 Method Not Allowed");
	isFileExist(custom) ? _content = readFile(custom) : _content = readFile("./conf_web/error/basico/405.html");
	Log::warn(RESP, "405 Method not allowed!");
}

void HTTPRes::error404()
{
	std::string custom = _config->getErrorPage(404);
	_header.addOne(_request.getHeader("Version"), "404 Not Found");
	isFileExist(custom) ? _content = readFile(custom) : _content = readFile("./conf_web/error/basico/404.html");
	Log::warn(GET, "404 Not Found!");
}

void HTTPRes::error403()
{
	std::string custom = _config->getErrorPage(403);
	_header.addOne(_request.getHeader("Version"), "403 Forbidden");
	isFileExist(custom) ? _content = readFile(custom) : _content = readFile("./conf_web/error/basico/403.html");
	Log::warn(GET, "403 Forbidden!");
}

std::string const HTTPRes::getHeader() const { return _header.getHeader(); }

std::string const HTTPRes::getContent() const { return _content; }
