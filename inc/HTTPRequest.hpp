/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 00:08:38 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/22 00:14:38 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include "Colors.hpp"

int start(void);

class HTTPRequest
{
private:
    std::map<std::string, std::string> _map;
    HTTPRequest();
    HTTPRequest &operator=(const HTTPRequest &);

public:
    HTTPRequest(const char *);
    ~HTTPRequest();
    const std::string &getHeader(const std::string name) const;

    const std::string &getPath() const;
    const std::string &getProt() const;
    const std::string &getMethod() const;
    const std::map<std::string, std::string> &getMap() const;

    void print();
};

#endif
