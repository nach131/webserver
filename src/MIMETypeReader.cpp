/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MIMETypeReader.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 20:08:48 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/06/02 13:18:44 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MIMETypeReader.hpp"

MIMETypeReader::MIMETypeReader(const std::string &filePath)
{
	loadMIMETypes(filePath);
}

std::string MIMETypeReader::getContentType(const std::string &extension) const
{
	std::string ext = extension;
	std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
	std::map<std::string, std::string>::const_iterator it = _mimeTypes.find(ext);
	if (it != _mimeTypes.end())
	{
		std::string mime = it->second;
		if (mime.substr(0, 4) == "text")
			mime += "; charset=UTF-8";
		return mime;
	}
	else
		return "application/octet-stream";
}

void MIMETypeReader::print()
{
	for (std::map<std::string, std::string>::iterator it = _mimeTypes.begin(); it != _mimeTypes.end(); it++)
		std::cout << it->first << " " << it->second << std::endl;
}

void MIMETypeReader::loadMIMETypes(const std::string &filePath)
{
	std::ifstream file(filePath.c_str());
	if (!file.is_open())
		throw std::runtime_error("mime type file not opened correctly!");

	std::string line;
	while (std::getline(file, line))
	{
		if (line.find("types") != std::string::npos)
		{
			while (std::getline(file, line) && line.find("}") == std::string::npos)
			{
				if (line.empty())
					continue;
				std::istringstream iss(line);
				std::string type;
				std::vector<std::string> extensions;
				iss >> type;
				std::string extension;
				while (iss >> extension)
					extensions.push_back(extension);
				for (std::vector<std::string>::iterator ext_it = extensions.begin(); ext_it != extensions.end(); ++ext_it)
					_mimeTypes[*ext_it] = type;
			}
			break;
		}
	}
	file.close();
}

MIMETypeReader::~MIMETypeReader() {}
