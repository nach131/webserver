/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogFile.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:05:47 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/30 19:27:26 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LogFile.hpp"

LogFile::LogFile()
{
	_file.open(".tmpdir/.log", std::ios::app);
	if (!_file.good())
		throw std::runtime_error("logfile not opened!");
}

LogFile::~LogFile()
{
	_file.close();
}

void LogFile::print(std::string info) { _file << info; }
