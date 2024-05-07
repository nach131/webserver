/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:10:22 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/07 12:52:46 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGSERVER_H__
# define CONFIGSERVER_H__

# include <map>
# include <vector>
# include <fstream>
# include <sstream>
# include <iostream>
# include "../Colors.hpp"
# include "../MIMETypeReader.hpp"

//class MIMETypeReader;

class Location
{
	private:
		Location();
		std::map<std::string, std::string> _val;

	public:
		Location(const std::vector<std::string> &, size_t &);
		~Location();
};

class ConfigServer
{
	private:
		bool _first;
		int _port;
		int _apiPort;
		std::string _serverName;
		std::string _rootDirectory;
		std::string _apiForward;
		std::string _prePath;
		std::map<int, std::string> _errorPages;
		std::map<std::string, std::map<std::string, std::string> > _locations;
		std::map<std::string, std::string> _gen;
		std::multimap<std::string, Location *> _loc;
		MIMETypeReader _mime;

	public:
		ConfigServer(const std::string &mimeFilePath);
		~ConfigServer();
		
		void loadConf(const std::string &filename);
		void print() const;
		
		int getPort() const;
		std::string getServerName() const;
		std::string getRootDirectory() const;
		std::string getErrorPage(int errorCode) const;
		std::string getApiForward() const;
		int getApiPort() const;
		std::map<std::string, std::map<std::string, std::string> > getLocation() const;
		std::string getContentType(const std::string &extension) const;
		
		bool getFirst() const;
		std::string getPrePath() const;
		
		void setFirst(bool action);
		void setPrePath(const std::string &path);

		class ConfErrorException : std::runtime_error
		{
			public:
				ConfErrorException(const std::string &);
		};
		typedef ConfigServer::ConfErrorException e_cee;
};

#endif
