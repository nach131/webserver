/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KeyValue.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:42:46 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/16 16:04:19 by vduchi           ###   ########.fr       */
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

	bool checkKey(std::string &key);
	bool checkValue(std::string &key, std::string &val);
	bool checkInt(std::string &val);
	bool checkString(std::string &val);
	bool checkLocation(std::string &val);
	void checkComplex(std::string &key, std::string &val);
};

#endif
