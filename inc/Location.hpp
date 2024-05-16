/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:42:46 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/16 13:14:36 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_H__
#define LOCATION_H__

#include "map"
#include "iostream"

class Location
{
private:
	Location();
	std::string _root;
	std::string _auto;
	std::string _index;
	std::string _methods;
	std::map<std::string, std::string> _cgi;

public:
	Location(const std::vector<std::string> &, size_t &);
	~Location();
};

#endif
