/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:05:47 by vduchi            #+#    #+#             */
/*   Updated: 2024/06/02 13:05:59 by vduchi           ###   ########.fr       */
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

Log::~Log() { instance.file.close(); }

void Log::error(int name, std::string info)
{
	char buf[64];
	std::string err(RED);
	std::stringstream ss;
	std::time_t now = std::time(nullptr);
	std::strftime(buf, 64, "%A %b %e %Y %H:%M:%S", std::localtime(&now));
	ss << buf;
	err += ss.str();
	ss << "\tERROR:\t";
	err += " ERROR ";
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
	case 6:
		err += "PYTHON -> ";
		ss << "PYTHON\t -> ";
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
	err += ss.str();
	ss << "\tINFO:\t";
	err += " INFO ";
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
	case 6:
		err += "PYTHON -> ";
		ss << "PYTHON\t -> ";
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
	err += ss.str();
	ss << "\tWARN:\t";
	err += " WARN ";
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
	case 6:
		err += "PYTHON -> ";
		ss << "PYTHON\t -> ";
		break;
	}
	ss << info;
	ss << "\n";
	err += info + "\n";
	std::cout << err;
	instance.file << ss.str() << std::flush;
}
