/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 00:08:38 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/30 16:44:49 by vduchi           ###   ########.fr       */
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

int start(void);

class HTTPRequest
{
private:
    bool _last;
    // char *_byteContent;
    std::string _fileType;
    std::string _fileName;
    std::string _boundary;
    std::map<std::string, std::string> _map;
    // HTTPRequest &operator=(const HTTPRequest &);

public:
    HTTPRequest();
    HTTPRequest(const HTTPRequest &);
    ~HTTPRequest();

    void setMulti(bool &);
    std::string getFileName();
    std::string getFileType();
    void findFileName(const char *);
    void takeHeader(std::stringstream &, int &);
    void checkLastBoundary(const char *, int &, int);
    void checkFirstBoundary(std::vector<std::string> &, int &);
    void getBuffer(const char *, int, bool &);

    std::string getLocation();
    const std::map<std::string, std::string> &getMap() const;
    const std::string &getHeader(const std::string name) const;

    void print();
    void cleanObject();
};

#endif
