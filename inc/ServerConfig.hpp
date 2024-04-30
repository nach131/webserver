/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:42:46 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/30 13:34:12 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <iostream>
#include <map>

#include "MIMETypeReader.hpp"

class MIMETypeReader;

class ServerConfig{

private:
    int _port;
    std::string _serverName;
    std::string _rootDirectory;
    std::map<int, std::string> _errorPages; 
    int _apiPort;
    std::string _apiForward;
    std::map<std::string, std::map<std::string, std::string> >  _locations; 
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
    std::string getContentType(const std::string &extension) const ;

};


#endif
