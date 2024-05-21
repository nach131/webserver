/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileContent.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:32:24 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/21 13:30:54 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILECONTENT_HPP
#define FILECONTENT_HPP

#include "ServerConfig.hpp"
// #include <vector>
// #include <fstream>
// #include <sstream>
// #include <iostream>

class FileContent
{
private:
	std::vector<std::string> _content;

public:
	FileContent(const char *);
	~FileContent();

	int checkLine(std::string &line);
	int checkServerKey(std::string &line);

	void error(const std::string, int);
	void createServers(std::vector<ServerConfig *> &);
	void checkKeysAndValues(std::vector<std::string> &, int &);
	void createOneServer(std::vector<ServerConfig *> &, std::vector<std::string> &);

	std::vector<std::string> checkOneServer(int &, int &);

	class ConfErrorException : public std::runtime_error
	{
	public:
		ConfErrorException(const std::string &);
	};
	typedef FileContent::ConfErrorException e_cee;
};

#endif
