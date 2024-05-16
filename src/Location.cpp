/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:42:46 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/16 17:42:12 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(std::vector<std::string> &arr, size_t &i) : _root(""), _auto(""), _index(""), _methods("")
{
	_words["root"] = 0;
	_words["index"] = 1;
	_words["cgi_ext"] = 2;
	_words["cgi_path"] = 3;
	_words["autoindex"] = 4;
	_words["allow_methods"] = 5;
	// (void)vec;
	// (void)size;
	// _root = "./dist";
	// _auto = "off";
	// _index = "ciao.html";
	// _methods = "DELETE POST GET";
	// _cgi["path"] = "/usr/bin/vattelapesca";
	for (i++; findClosure(arr[i]); i++)
	{
		std::string el, key, value;
		std::stringstream ss(arr[i]);
		ss >> key;
		while (ss >> el)
		{
			if (value[0] == '\0')
				value.append(el);
			else
				value.append(" " + el);
		}
		takeOutSemiColumn(value);
		std::cout << "Location key: " << key << " Val: " << value << std::endl;
		setValue(key, value);
	}
}

Location::~Location() {}

void Location::setValue(std::string &key, std::string &value)
{
	std::cout << "Switch value: " << key << " " << _words[key] << std::endl;
	switch (_words[key])
	{
	case root:
		_root = value;
		break;
	case file_index:
		_index = value;
		break;
	case cgi_ext:
		_cgi["cgi_ext"] = value;
		break;
	case cgi_path:
		_cgi["cgi_path"] = value;
		break;
	case autoindex:
		_auto = value;
		break;
	case methods:
		_methods = value;
		break;
	}
}

std::string Location::getRoot() { return _root; }

std::string Location::getIndex() { return _index; }

std::string Location::getMethods() { return _methods; }

std::string Location::getAutoIndex() { return _auto; }

std::map<std::string, std::string> Location::getCGI() { return _cgi; }

bool Location::findClosure(std::string &val)
{
	bool check = false;
	for (size_t i = 0; i < val.length(); i++)
	{
		if (val[i] != ' ' && val[i] != '\t' && val[i] != '}')
			check = true;
	}
	return check;
}
