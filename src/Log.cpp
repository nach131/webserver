/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:05:47 by vduchi            #+#    #+#             */
/*   Updated: 2024/06/01 12:02:40 by vduchi           ###   ########.fr       */
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
	switch (name)
	{
	case 0:
		ss << "SERVER\t -> ";
		break;
	case 1:
		ss << "REQUEST\t -> ";
		break;
	case 2:
		ss << "RESPONSE -> ";
		break;
	case 3:
		ss << "GET\t\t -> ";
		break;
	case 4:
		ss << "POST\t -> ";
		break;
	case 5:
		ss << "DELETE\t -> ";
		break;
	}
	ss << info;
	ss << "\n";
	err += ss.str();
	instance.file << ss.str() << std::flush;
	// instance.file.write(test.c_str(), test.length());
	throw std::runtime_error("");
}

void Log::info(int name, std::string info)
{
	char buf[64];
	std::string err(WHITE);
	std::stringstream ss;
	std::time_t now = std::time(nullptr);
	std::strftime(buf, 64, "%A %b %e %Y %H:%M:%S", std::localtime(&now));
	ss << buf;
	ss << "\tINFO:\t";
	switch (name)
	{
	case 0:
		ss << "SERVER\t -> ";
		break;
	case 3:
		ss << "GET\t\t -> ";
		break;
	case 4:
		ss << "POST\t -> ";
		break;
	case 5:
		ss << "DELETE\t -> ";
		break;
	}
	ss << info;
	ss << "\n";
	err += ss.str();
	std::cout << err;
	instance.file << ss.str() << std::flush;
	// instance.file.write(err.c_str(), err.length());
	// throw std::runtime_error("");
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
	switch (name)
	{
	case 2:
		ss << "RESPONSE -> ";
		break;
	case 3:
		ss << "GET\t\t -> ";
		break;
	case 4:
		ss << "POST\t -> ";
		break;
	case 5:
		ss << "DELETE\t -> ";
		break;
	}
	ss << info;
	ss << "\n";
	err += ss.str();
	std::cout << err;
	instance.file << err << std::flush;
	// throw std::runtime_error("");
}
