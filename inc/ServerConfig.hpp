/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:42:46 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/06/02 13:21:58 by vduchi           ###   ########.fr       */
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
	int _sockFd;
	int *_data;
	long _maxBody;
	std::string _apiForward;
	std::string _serverName;
	std::string _rootDirectory;
	std::map<int, std::string> _errorPages;
	std::map<std::string, std::map<std::string, std::string> > _locations;
	KeyValue _kv;
	MIMETypeReader _mime;

public:
	ServerConfig(const std::vector<std::string> &, const std::string &, int);
	~ServerConfig();

	// void print() const;
	int getData();
	void setData(int *);
	int getPort() const;
	int getServerSocket() const;
	void setServerSocket(const int);
	std::string getServerName() const;
	std::string getErrorPage(int errorCode) const;
	std::string getContentType(const std::string &extension) const;
	LocationResult getLocationConfig(const std::string &path);

	int checkLine(const std::string &line);
	void addValue(std::stringstream &, int);
	void fillVariables(const std::vector<std::string> &, int);

	void error(const std::string, int);
	class ConfErrorException : public std::runtime_error
	{
	public:
		ConfErrorException(const std::string &);
	};
	typedef ServerConfig::ConfErrorException e_cee;
};

#endif
