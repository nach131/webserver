/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 00:08:38 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/26 17:38:59 by nmota-bu         ###   ########.fr       */
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
    std::string _boundary;
    std::string _buffer;
    std::map<std::string, std::string> _map;
    // HTTPRequest &operator=(const HTTPRequest &);

public:
    HTTPRequest();
    HTTPRequest(const char *);
    ~HTTPRequest();
    void getBuffer(const char *, bool &);
    const std::string &getHeader(const std::string name) const;
     std::string getLocation() ;

    const std::map<std::string, std::string> &getMap() const;

    const std::string &getBuffer() const;

    void print();
};

#endif
