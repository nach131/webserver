/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KeyValue.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:42:46 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/14 19:21:49 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "KeyValue.hpp"

KeyValue::KeyValue()
{
	_keys.push_back("root");
	_keys.push_back("index");
	_keys.push_back("listen");
	_keys.push_back("api_port");
	_keys.push_back("autoindex");
	_keys.push_back("error_page");
	_keys.push_back("server_name");
	_keys.push_back("allow_methods");
	_keys.push_back("client_max_body_size");
	_keys.push_back("cgi_ext");
	_keys.push_back("cgi_path");
}

KeyValue::~KeyValue()
{
	_keys.clear();
	_pairs.clear();
}

bool KeyValue::checkKey(std::string &key)
{
	std::vector<std::string>::iterator it = _keys.begin();
	for (; it != _keys.end(); it++)
		if (*it == key)
			return true;
	return false;
}

bool KeyValue::checkValue(std::string &key, std::string &val)
{
	bool check = false;
	std::multimap<std::string, std::string>::iterator it = _pairs.begin();
	for (; it != _pairs.end(); it++)
	{
		if (key == it->first && value == std::type(std::string))
		{
		}
	}
	return check;
}