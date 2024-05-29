/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogFile.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:05:47 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/29 11:50:37 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LogFile.hpp"

LogFile::LogFile()
{
	_file.open("./conf_web/.log");
	if (!_file.good())
		throw std::runtime_error("logfile not opened!");
}

LogFile::~LogFile()
{
	_file.close();
}
