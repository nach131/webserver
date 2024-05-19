/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:42:46 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/19 16:10:17 by vduchi           ###   ########.fr       */
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
		bool _first;
		int _port;
		int _apiPort;
		int _serverSocketFd;
		char *_buffer;
		struct sockaddr_in _serverAddress;
		std::string _serverName;
		std::string _rootDirectory;
		std::string _apiForward;
		std::string _prePath;
		std::map<int, std::string> _errorPages;
		std::map<std::string, std::map<std::string, std::string> > _locations;
		std::map<std::string, std::string> _gen;
		std::map<std::string, std::map<std::string, std::string> > _loc;
		KeyValue _kv;
		MIMETypeReader _mime;

	public:
		ServerConfig(const std::string &mimeFilePath);
		~ServerConfig();
		
		void loadConf(const std::string &filename);
		void print() const;
		void printLocations();

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

		bool getFirst() const;
		std::string getPrePath() const;

		void setFirst(bool);
		void setPrePath(const std::string &);

		int checkLine(std::string &);
		bool findClosure(std::string &);
		void checkSyntax(std::vector<std::string> &);

		void fillVariables(std::vector<std::string> &);

		class ConfErrorException : public std::runtime_error
		{
			public:
				ConfErrorException(const std::string &);
		};
		typedef ServerConfig::ConfErrorException e_cee;
};

#endif
