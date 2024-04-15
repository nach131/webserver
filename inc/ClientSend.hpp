/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSend.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 12:12:11 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/15 11:44:56 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSEND_HPP
#define CLIENTSEND_HPP

#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

class ClientSend
{
private:
	std::string _content;
	ClientSend &operator=(const ClientSend &);

	ClientSend(const ClientSend &);

	static int methodGet(ClientSend &client, std::string path, std::string prot);
	static int methodPost(ClientSend &client, std::string path, std::string prot);
	static int methodDelete(ClientSend &client, std::string path, std::string prot);

public:
	// ClientSend(const std::map<std::string, std::string> &);
	ClientSend(const std::string &, const std::string &, const std::string &);

	std::string const &content() const;
	// std::string const &content();

	~ClientSend();
};

#endif
