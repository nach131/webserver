/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 00:08:38 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/20 20:56:46 by vduchi           ###   ########.fr       */
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
    std::map<std::string, std::string> _map;
    HTTPRequest &operator=(const HTTPRequest &);

public:
    HTTPRequest();
    // HTTPRequest(const char *);
    ~HTTPRequest();
    void getRequest(const char *);
    const std::string &getHeader(const std::string name) const;
     std::string getLocation() ;

    const std::map<std::string, std::string> &getMap() const;

    void print();
};

#endif
