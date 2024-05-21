/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KeyValue.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:42:46 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/21 13:30:50 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYVALUE_HPP
#define KEYVALUE_HPP

#include "WebServer.hpp"
#include <map>
#include <vector>

class KeyValue
{
private:
	std::vector<std::string> _keys;
	std::map<std::string, std::string> _pairs;

public:
	KeyValue();
	~KeyValue();

	std::vector<std::string> getKeys();

	bool checkKey(std::string &);
	bool checkValue(std::string &, std::string &);
	bool checkComplex(std::string &, std::string &, int);

	void error(const std::string, int);
	std::string checkType(std::string &);

	class KeyValueException : public std::runtime_error
	{
	public:
		KeyValueException(const std::string &);
	};
	typedef KeyValue::KeyValueException e_kve;
};

#endif
