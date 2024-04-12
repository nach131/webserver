/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSend.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 12:12:11 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/12 13:43:02 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSEND_HPP
#define CLIENTSEND_HPP

#include <map>
#include <iostream>

class ClientSend
{
private:
	ClientSend &operator=(const ClientSend &);
	ClientSend(const ClientSend &);

public:
	ClientSend(const std::map<std::string, std::string> &);
	ClientSend(const std::string &, const std::string &, const std::string &);

	~ClientSend();
};

#endif
