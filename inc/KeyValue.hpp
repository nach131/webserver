/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KeyValue.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:42:46 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/14 18:45:45 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYVALUE_HPP
#define KEYVALUE_HPP

#include <map>
#include <vector>
#include <iostream>

class KeyValue
{
private:
	std::vector<std::string> _keys;
	std::multimap<std::string, std::string> _pairs;

public:
	KeyValue();
	~KeyValue();

	bool checkKey(std::string &);
	bool checkValue(std::string &, std::string &);
};

#endif