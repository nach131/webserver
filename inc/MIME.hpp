/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MIME.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:30:49 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/18 14:58:35 by nmota-bu         ###   ########.fr       */
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
	_mime["shhtml"] = "text/html";

	_mime["xml"] = "text/xml";
	_mime["rss"] = "text/xml";
	_mime["htc"] = "text/x-component";
	_mime["mml"] = "text/mathml";

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
	_mime["jng"] = "image/x-jng";
	_mime["wbmp"] = "image/vnd.wap.wbmp";

	_mime["mp3"] = "audio/mp3";
	_mime["wav"] = "audio/wav";
	_mime["mp4"] = "video/mp4";
	_mime["avi"] = "video/x-msvideo";
	_mime["mov"] = "video/quicktime";
	_mime["flv"] = "video/quicktime";

	_mime["doc"] = "application/msword";
	_mime["jar"] = "application/java-archive";
	_mime["war"] = "application/java-archive";
	_mime["ear"] = "application/java-archive";
	_mime["gz"] = "application/x-gzip";
	_mime["pdf"] = "application/pdf";
	_mime["json"] = "application/json";
	_mime["hqx"] = "application/mac-binhex40";
	_mime["cco"] = "application/x-cocoa";
	_mime["jardiff"] = "application/x-java-archive-diff";
	_mime["jnlp"] = "application/x-java-jnlp-file";
	_mime["run"] = "application/x-makeself";
	_mime["pl"] = "application/x-perl";
	_mime["pm"] = "application/x-perl";
	_mime["prc"] = "application/x-pilot";
	_mime["pdb"] = "application/x-pilot";
	_mime["rar"] = "application/x-rar-compressed";
	_mime["zip"] = "application/zip";
	_mime["rpm"] = "application/x-redhat-package-manager";
	_mime["sea"] = "application/x-sea";
	_mime["swf"] = "application/x-shockwave-flash";
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
