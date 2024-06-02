/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PythonExec.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 10:02:31 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/06/02 13:36:40 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PythonExec.hpp"
#include <cstdlib>
#include <fstream>
#include <sstream>

void sleep_ms(int milliseconds)
{
	struct timespec ts;
	ts.tv_sec = milliseconds / 1000;
	ts.tv_nsec = (milliseconds % 1000) * 1000000;
	nanosleep(&ts, NULL);
}

std::string execPython(const std::string &py)
{
	std::string result;
	std::string command = "python3 " + py + " > ./.tmpdir/.tmp";
	int returnCode = std::system(command.c_str());
	if (returnCode != 0)
		Log::error(PY, "Python execution error!");
	std::ifstream tempFile("./.tmpdir/.tmp");
	if (!tempFile.good())
		Log::error(PY, "Cannot read temporary file!");
	std::stringstream ss;
	ss << tempFile.rdbuf();
	result = ss.str();
	return result;
}

std::string execPython(const std::string &py, const std::string &data)
{
	std::string result;
	std::string dataTmp = data;
	removeLastNewline(dataTmp);
	std::string command = "python3 " + py + " \"" + dataTmp + "\" > ./.tmpdir/.tmp";
	int returnCode = std::system(command.c_str());
	if (returnCode != 0)
		Log::error(PY, "Python execution error!");
	std::ifstream tempFile("./.tmpdir/.tmp");
	if (!tempFile.good())
		Log::error(PY, "Cannot read temporary file!");
	std::stringstream ss;
	ss << tempFile.rdbuf();
	result = ss.str();
	return result;
}

std::string execPython(const std::string &py, const std::string &dirPath, const std::string &root_location)
{
	std::string result;
	std::string command = "python3 " + py + " " + dirPath + " " + root_location + " > ./.tmpdir/.tmp";
	int returnCode = std::system(command.c_str());
	if (returnCode != 0)
		Log::error(PY, "Python execution error!");
	std::ifstream tempFile("./.tmpdir/.tmp");
	if (!tempFile.good())
		Log::error(PY, "Cannot read temporary file!");
	std::stringstream ss;
	ss << tempFile.rdbuf();
	result = ss.str();
	return result;
}
