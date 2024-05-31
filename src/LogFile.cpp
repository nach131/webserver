/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogFile.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:05:47 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/31 13:00:38 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

LogFile LogFile::instance;

LogFile::LogFile()
{
	instance.file.open(".tmpdir/.log", std::ios::out);
	if (!file.good())
		throw std::runtime_error("logfile not opened!");
}

LogFile::~LogFile()
{
	instance.file.close();
}

void LogFile::print(std::string name, std::string info)
{
	instance.file << name << info;
}
