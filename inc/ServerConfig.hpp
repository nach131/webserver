/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:42:46 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/21 12:34:48 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "KeyValue.hpp"
#include <string.h>
#include <netinet/in.h>
#include "MIMETypeReader.hpp"
#include "Location.hpp"

// class MIMETypeReader;

class ServerConfig
{

private:
	int _port;
	int _apiPort;
	int _serverSocketFd;
	char *_buffer;
	std::string _apiForward;
	std::string _serverName;
	std::string _rootDirectory;
	std::map<int, std::string> _errorPages;
	std::map<std::string, std::map<std::string, std::string>> _locations;
	struct sockaddr_in _serverAddress;
	KeyValue _kv;
	MIMETypeReader _mime;

public:
	ServerConfig(const std::vector<std::string> &, const std::string &);
	~ServerConfig();

	// std::string getRoot(const std::string &location) const;
	// std::string getIndex(const std::string &location) const;
	// bool isLocationAllowed(const std::string &path);
	// bool isMethodAllowed(const std::string &path, const std::string &method);

	LocationResult getLocationConfig(const std::string &path);

	void print() const;
	void printLocations();

	// int getApiPort() const;
	// std::string getApiForward() const;
	int getPort() const;
	int getServerSocket() const;
	std::string getServerName() const;
	std::string getRootDirectory() const;
	std::string getErrorPage(int errorCode) const;
	std::string getContentType(const std::string &extension) const;
	// std::map<std::string, std::map<std::string, std::string>> getLocation() const;

	char *getBuffer() const;
	void setBuffer(char *buf);
	// struct sockaddr_in getServerAddress() const; No se si es necesario

	int checkLine(const std::string &line);

	void addValue(std::stringstream &);
	void fillVariables(const std::vector<std::string> &);
};

#endif
