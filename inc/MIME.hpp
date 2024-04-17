/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MIME.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:30:49 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/17 21:22:18 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIME_HPP
#define MIME_HPP

#include <map>
#include <string>

class MIME
{
private:
	typedef std::map<std::string, std::string> MimeMap;
	MimeMap _mime;

public:
	MIME();
	~MIME();

	std::string getMIME(const std::string &extension) const;
};

MIME::MIME()
{
	_mime["html"] = "text/html";
	_mime["htm"] = "text/html";
	_mime["css"] = "text/css";
	_mime["txt"] = "text/plain";
	_mime["js"] = "text/javascript";
	_mime["ico"] = "image/x-icon";
	_mime["bmp"] = "image/bmp";
	_mime["svg"] = "image/svg+xml";
	_mime["gif"] = "image/gif";
	_mime["png"] = "image/png";
	_mime["jpg"] = "image/jpeg";
	_mime["jpeg"] = "image/jpeg";
	_mime["png"] = "image/png";
	_mime["mp3"] = "audio/mp3";
	_mime["wav"] = "audio/wav";
	_mime["mp4"] = "video/mp4";
	_mime["avi"] = "video/avi";
	_mime["avi"] = "video/x-msvideo";
	_mime["doc"] = "application/msword";
	_mime["gz"] = "application/x-gzip";
	_mime["pdf"] = "application/pdf";
	_mime["json"] = "application/json";
}

MIME::~MIME()
{
}

std::string MIME::getMIME(const std::string &extension) const
{
	MimeMap::const_iterator it = _mime.find(extension);
	if (it != _mime.end())
		return it->second;
	else
		return _mime.find("default")->second;
}

// int main()
// {
// 	MIME mime;
// 	std::string extension = "html";
// 	std::string mimeType = mime.getMIME(extension);
// 	return 0;
// }

#endif
