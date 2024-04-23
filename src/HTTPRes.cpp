/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRes.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:54:23 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/23 11:13:21 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRes.hpp"

HTTPRes::HTTPRes(const HTTPRequest &request) : _request(request)
{

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
	MIMETypeReader mime("./conf_web/mime.type");

	std::string path = res._request.getHeader("Path");
	std::string extension = getExtension(path);

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
		res._header.addField("Content-Type", mime.getContentType(extension));

	res._header.addField("Content-Length", std::to_string(res._content.length()));
	res._header.addField("Date", DateField());
	res._header.addField("ETag", generateETag(res._content));
	res._header.addField("42-Barcelona", "nmota-bu, vduchi");
}

void HTTPRes::methodPost(HTTPRes &res)
{
	(void)res;
	std::cout << "==========POST==========\n";
	res._header.addOne(res._request.getHeader("Version"), "200 OK");

	res._header.addField("Host", "localhost:3000/api/users");
	res._header.addField("Content-Type", "application/json; charset=utf-8");
	res._header.addField("Date", DateField());
	res._header.addField("42-Barcelona", "nmota-bu, vduchi");
	// res._header.addField("", "\r\n\r\n");
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
