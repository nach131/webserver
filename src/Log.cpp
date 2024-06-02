/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:05:47 by vduchi            #+#    #+#             */
/*   Updated: 2024/06/02 12:36:40 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

Log Log::instance;

Log::Log()
{
	file.open(".tmpdir/.log", std::ios::in | std::ios::out);
	if (!file.good())
		throw std::runtime_error("logfile not good!");
	else if (!file.is_open())
		throw std::runtime_error("logfile not opened!");
}

Log::~Log()
{
	instance.file.close();
}

void Log::error(int name, std::string info)
{
	char buf[64];
	std::string err(RED);
	std::stringstream ss;
	std::time_t now = std::time(nullptr);
	std::strftime(buf, 64, "%A %b %e %Y %H:%M:%S", std::localtime(&now));
	ss << buf;
	ss << "\tERROR:\t";
	err += ss.str();
	switch (name)
	{
	case 0:
		err += "SERVER -> ";
		ss << "SERVER\t -> ";
		break;
	case 1:
		err += "REQUEST -> ";
		ss << "REQUEST\t -> ";
		break;
	case 2:
		err += "RESPONSE -> ";
		ss << "RESPONSE -> ";
		break;
	case 3:
		err += "GET -> ";
		ss << "GET\t\t -> ";
		break;
	case 4:
		err += "POST -> ";
		ss << "POST\t -> ";
		break;
	case 5:
		err += "DELETE -> ";
		ss << "DELETE\t -> ";
		break;
	}
	ss << info;
	ss << "\n";
	err += info + "\n";
	std::cout << err;
	instance.file << ss.str() << std::flush;
	throw std::runtime_error("");
}

void Log::info(int name, std::string info)
{
	char buf[64];
	std::string err(CYAN);
	std::stringstream ss;
	std::time_t now = std::time(nullptr);
	std::strftime(buf, 64, "%A %b %e %Y %H:%M:%S", std::localtime(&now));
	ss << buf;
	ss << "\tINFO:\t";
	err += ss.str();
	switch (name)
	{
	case 0:
		err += "SERVER -> ";
		ss << "SERVER\t -> ";
		break;
	case 3:
		err += "GET -> ";
		ss << "GET\t\t -> ";
		break;
	case 4:
		err += "POST -> ";
		ss << "POST\t -> ";
		break;
	case 5:
		err += "DELETE -> ";
		ss << "DELETE\t -> ";
		break;
	}
	ss << info;
	ss << "\n";
	err += info + "\n";
	std::cout << err;
	instance.file << ss.str() << std::flush;
}

void Log::warn(int name, std::string info)
{
	char buf[64];
	std::string err(YELLOW);
	std::stringstream ss;
	std::time_t now = std::time(nullptr);
	std::strftime(buf, 64, "%A %b %e %Y %H:%M:%S", std::localtime(&now));
	ss << buf;
	ss << "\tWARN:\t";
	err += ss.str();
	switch (name)
	{
	case 2:
		err += "RESPONSE -> ";
		ss << "RESPONSE -> ";
		break;
	case 3:
		err += "GET -> ";
		ss << "GET\t\t -> ";
		break;
	case 4:
		err += "POST -> ";
		ss << "POST\t -> ";
		break;
	case 5:
		err += "DELETE -> ";
		ss << "DELETE\t -> ";
		break;
	}
	ss << info;
	ss << "\n";
	err += info + "\n";
	std::cout << err;
	instance.file << ss.str() << std::flush;
}
