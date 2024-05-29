/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogFile.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:05:47 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/29 11:50:35 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGFILE_HPP
#define LOGFILE_HPP

#include "WebServer.hpp"

class LogFile
{
private:
	std::ifstream _file;

public:
	LogFile();
	~LogFile();
};

#endif
