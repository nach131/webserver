/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 00:08:38 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/06/01 17:31:33 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include "Colors.hpp"
#include <vector>

// int start(void);

class HTTPRequest
{
private:
    bool _last;
    bool _firstBound;
    std::string _fileType;
    std::string _fileName;
    std::string _boundary;
    std::map<std::string, std::string> _map;

public:
    HTTPRequest();
    HTTPRequest(const HTTPRequest &);
    ~HTTPRequest();

    void setMulti(bool &);
    std::string getFileName();
    std::string getFileType();
    std::string getLocation();
    const std::map<std::string, std::string> &getMap() const;
    const std::string &getHeader(const std::string &name) const;

    void findFileName(const char *);
    void takeHeader(std::stringstream &, int &);
    void checkLastBoundary(const char *, int &, int &, int);
    void checkFirstBoundary(std::vector<std::string> &, int &);
    void takeBuffer(const char *, int, bool &, bool &);

    void print();
    void cleanObject();
};

#endif
