/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:42:46 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/16 17:42:04 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_H__
#define LOCATION_H__

#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include "WebServer.hpp"

enum
{
	root,
	file_index,
	cgi_ext,
	cgi_path,
	autoindex,
	methods
};

class Location
{
private:
	Location();
	std::string _root;
	std::string _auto;
	std::string _index;
	std::string _methods;
	std::map<std::string, int> _words;
	std::map<std::string, std::string> _cgi;

public:
	Location(std::vector<std::string> &, size_t &);
	~Location();

	bool findClosure(std::string &);
	void setValue(std::string &, std::string &);

	std::string getRoot();
	std::string getIndex();
	std::string getMethods();
	std::string getAutoIndex();
	std::map<std::string, std::string> getCGI();
};

#endif
