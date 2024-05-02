/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 13:36:30 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/02 21:43:15 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tmp/ConfigFile.hpp"

/*
void ConfigFile::takeValues(std::vector<std::string> & arr, size_t & i)
{
	int brack = 1;

	for (; i < arr.size() && brack != 0; i++)
	{
		std::string key, val;
		std::stringstream ss(arr[i]);
		ss >> key;
		// TODO: Controlar que la key este' bien
		while (ss >> val)
		{
			// TODO: Controlar que los valores esten bien
		}
	}
}
*/

void ConfigFile::checkSyntax(std::vector<std::string> & arr, int & lineNum, size_t & i)
{
	int len = 0;
	int brack = 0;

	if (arr[i].find("server {") == std::string::npos)
		throw std::runtime_error("server declaration incorrect at line " + lineNum);
	len++;
	brack++;
	lineNum++;
	for (size_t j = i; j < arr.size() && arr[j].find("server {") == std::string::npos; j++)
		len++;
	for (size_t j = i + 1; j < i + len; j++)
	{
		if (arr[j].find(";") == std::string::npos
			&& arr[j].find("location {") == std::string::npos)
			throw std::runtime_error("expected semicolon at line " + lineNum);
		for (size_t l = 0; l < arr[j].length(); l++)
		{
			if (arr[j][l] == '{') { brack++; }
			if (arr[j][l] == '}') { brack--; }
			if (brack < 0) { throw std::runtime_error("expected block before } at line " + lineNum); }
			if (arr[j][l] == ';') { break; }
		}
		lineNum++;
	}
	if (brack > 0) { throw std::runtime_error("server block not closed at line " + lineNum); }
	i++;
}

ConfigFile::ConfigFile(const std::string & filename)
{
	int lineNum = 1;
	std::string line;
	std::ifstream in(filename);
	std::vector<std::string> arr;

	while (getline(in, line))
		arr.push_back(line);
	for (size_t i = 0; i < arr.size(); i++)
	{
		if (arr[i].find("server") != std::string::npos)
		{
			try {
				checkSyntax(arr, lineNum, i);
				this->_srvs[this->_srvs.size()] = new ConfigServer(arr, i);
			}
			catch (std::runtime_error & ex) { throw e_cee(ex.what()); }
		}
		else
		{
			if (arr[i].find(";") == std::string::npos)
				throw e_cee("expected semicolon at line " + lineNum);
//			std::string key, val;
//			std::stringstream ss(arr[i]);
			// TODO: Controlar si la key existe Si no existe, devolver error, si existe cojer los valores y comprobarlos
			// TODO: Anadir todo al mapa general
		}
		lineNum++;
	}
}

ConfigFile::~ConfigFile()
{
	_gen.clear();
	for (std::map<int, ConfigServer *>::iterator it = _srvs.begin(); it != _srvs.end(); it++)
		it->second->~ConfigServer();
	_srvs.clear();
}

ConfigFile::ConfErrorException::ConfErrorException (const std::string & msg)
	: std::runtime_error("\033[0;31mError: " + msg) {}
