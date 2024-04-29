/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelona.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 13:36:30 by vduchi            #+#    #+#             */
/*   Updated: 2024/04/28 16:04:09 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ConfigFile.hpp"

ConfigFile::ConfigFile(const std::string & filename)
{
	int lineNum = 1;
	std::string line;
	std::ifstream in(filename);
	std::vector<std::string> arr;
	while (getline(in, line))
		arr.push_back(line);
	for (std::string s : arr)
	{
		if (s.find("server") == std::string::npos)
			throw e_cee("Error at line " + std::to_string(lineNum) + " -> no semicolumn");
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