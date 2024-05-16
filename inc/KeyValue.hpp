/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KeyValue.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:42:46 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/16 14:59:38 by vduchi           ###   ########.fr       */
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
	std::map<std::string, std::string> _pairs;

public:
	KeyValue();
	~KeyValue();

	bool checkKey(std::string &);
	bool checkValue(std::string &, std::string &);
	bool checkInt(std::string &);
	bool checkString(std::string &);
	bool checkLocation(std::string &);
	bool checkComplex(std::string &, std::string &);
};

#endif
