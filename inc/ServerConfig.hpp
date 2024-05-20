/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:42:46 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/20 19:23:28 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

#include "KeyValue.hpp"
#include <string.h>
#include <netinet/in.h>
#include "MIMETypeReader.hpp"

//class MIMETypeReader;

class ServerConfig
{

private:
	int _port;
	int _apiPort;
	int _serverSocketFd;
	char *_buffer;
	std::string _serverName;
	std::string _rootDirectory;
	std::string _apiForward;
	std::string _prePath;
	struct sockaddr_in _serverAddress;
	std::map<int, std::string> _errorPages;
	std::map<std::string, std::string> _gen;
	std::map<std::string, std::map<std::string, std::string> > _locations;
	KeyValue _kv;
	MIMETypeReader _mime;

public:
	ServerConfig(const std::vector<std::string> &, const std::string &);
	~ServerConfig();

	void print() const;
	void printLocations();
//	void loadConf(const std::string &);

	int getPort() const;
	int getApiPort() const;
	int getServerSocket() const;
	std::string getApiForward() const;
	std::string getServerName() const;
	std::string getRootDirectory() const;
	std::string getErrorPage(int errorCode) const;
	std::string getContentType(const std::string &extension) const;
	std::map<std::string, std::map<std::string, std::string> > getLocation() const;

	char *getBuffer() const;
	void setBuffer(char *buf);
	// struct sockaddr_in getServerAddress() const; No se si es necesario

	int checkLine(const std::string &line);

	void fillVariables(const std::vector<std::string> &);
};

#endif
