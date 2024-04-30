/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientParsing.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:01:14 by vduchi            #+#    #+#             */
/*   Updated: 2024/04/29 18:35:16 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTMESSAGE_H__
#define CLIENTMESSAGE_H__

#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include "Colors.hpp"
#include "Colors.hpp"

int start(void);

class ClientParsing
{
private:
	std::map<std::string, std::string> _map;
	ClientParsing();
	ClientParsing &operator=(const ClientParsing &);

public:
	ClientParsing(const char *);
	~ClientParsing();
	const std::string &getHeader(const std::string name) const;

	const std::string &getPath() const;
	const std::string &getProt() const;
	const std::string &getMethod() const;
	const std::map<std::string, std::string> &getMap() const;

	void print();
};

#endif
