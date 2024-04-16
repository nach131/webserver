/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPError.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 17:31:23 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/16 17:32:55 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPERROR_HPP
#define HTTPERROR_HPP

#include <iostream>
#include <map>

class HTTPError
{
private:
	static std::map<int, std::string> errorMessages;

public:
	static void initialize()
	{
		errorMessages[200] = "OK";
		errorMessages[400] = "Bad Request";
		errorMessages[401] = "Unauthorized";
		errorMessages[403] = "Forbidden";
		errorMessages[404] = "Not Found";
		errorMessages[500] = "Internal Server Error";
	}

	static std::string getMessage(int code)
	{
		std::map<int, std::string>::iterator it = errorMessages.find(code);
		if (it != errorMessages.end())
			return it->second;
		else
			return "Unknown Error";
	}
};

#endif
