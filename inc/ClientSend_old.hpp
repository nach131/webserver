/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSend.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 12:12:11 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/15 20:03:11 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSEND_HPP
# define CLIENTSEND_HPP

# include <map>
# include <iostream>
# include <fstream>
# include <sstream>
# include "ClientParsing.hpp"

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
	ClientSend(const ClientParsing &);

	std::string const &content() const;
	// std::string const &content();

	~ClientSend();
};

#endif
