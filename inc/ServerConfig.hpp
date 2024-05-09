/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:42:46 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/08 19:36:41 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <map>
# include <vector>
# include <fstream>
# include <sstream>
# include <iostream>
# include "Colors.hpp"
# include "MIMETypeReader.hpp"

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

class ServerConfig
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
		ServerConfig(const std::string &mimeFilePath);
		~ServerConfig();
		
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

		int checkLine(std::string & line);
		void checkSyntax(std::vector<std::string> & arr);

		class ConfErrorException : std::runtime_error
		{
			public:
				ConfErrorException(const std::string &);
		};
		typedef ServerConfig::ConfErrorException e_cee;
};

#endif
