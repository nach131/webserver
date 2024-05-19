/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KeyValue.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:42:46 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/19 16:09:26 by vduchi           ###   ########.fr       */
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

	_pairs["root"] = "string";
	_pairs["index"] = "string";
	_pairs["listen"] = "int";
	_pairs["api_port"] = "int";
	_pairs["autoindex"] = "string";
	_pairs["error_page"] = "int";
	_pairs["server_name"] = "string";
	_pairs["allow_methods"] = "string";
	_pairs["client_max_body_size"] = "int";
	_pairs["cgi_ext"] = "string";
	_pairs["cgi_path"] = "string";
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
			return false;
	return true;
}

bool KeyValue::checkValue(std::string &key, std::string &val)
{
	bool check = false;
	std::map<std::string, std::string>::iterator it = _pairs.begin();
	for (; it != _pairs.end(); it++)
		if (key == it->first)
			check = checkType(val) == it->second ? false : true;
	return check;
}

bool KeyValue::checkComplex(std::string &key, std::string &val, int lineNum)
{
	int err = 0, count = 0;
	std::string el;
	if (key == "error_page")
	{
		std::stringstream s(val), ss(val);
		s >> el;
		if (checkType(el) == "string")
			parseError("error number is not a number at line ", lineNum);
		err = std::atoi(el.c_str());
		while (s >> el)
			count++;
		if (count > 1)
			parseError("too many arguments for error_page at line ", lineNum);
		s.str("");
		s.clear();
	}
	else
	{
		std::stringstream ss(val);
		while (ss >> el)
			if (el != "GET" && el != "POST" && el != "DELETE")
				parseError("method not allowed at line ", lineNum);
	}
	return false;
}

std::string KeyValue::checkType(std::string &val)
{
	bool isInt = true;
	for (size_t i = 0; i < val.length(); i++)
		if (val[i] < '0' || val[i] > '9')
			isInt = false;
	return isInt ? "int" : "string";
}
