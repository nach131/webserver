/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelona.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:10:22 by vduchi            #+#    #+#             */
/*   Updated: 2024/04/28 15:23:07 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGSERVER_H__
# define CONFIGSERVER_H__

# include <map>
# include <vector>
# include <fstream>
# include <sstream>
# include <iostream>
# include "Colors.hpp"

class Location
{
	private:
		Location();
		std::map<std::string, std::string> _val;

	public:
		Location(const std::string &);
		~Location();
};

class ConfigServer
{
	private:
		ConfigServer();
		std::map<std::string, std::string> _gen;
		std::multimap<std::string, Location *> _loc;

	public:
		ConfigServer(const std::string &);
		~ConfigServer();
};

#endif
