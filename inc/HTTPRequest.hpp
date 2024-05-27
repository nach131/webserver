/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 00:08:38 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/27 16:43:31 by vduchi           ###   ########.fr       */
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
    void takeHeader(std::stringstream &);
    void checkBoundary(std::vector<std::string> &);
    void getBuffer(const char *, bool &);

    std::string getLocation();
    const std::map<std::string, std::string> &getMap() const;
    const std::string &getHeader(const std::string name) const;

    void print();
    void cleanObject();
};

#endif
