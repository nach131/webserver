/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:44:13 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/16 10:46:46 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

std::string executePythonScript(const std::string &scriptPath)
{
	std::string result;
	FILE *pipe = popen(("python3 " + scriptPath).c_str(), "r");
	if (!pipe)
	{
		std::cerr << "Error: Failed to open pipe for Python script execution." << std::endl;
		return result;
	}

	char buffer[128];
	while (fgets(buffer, sizeof(buffer), pipe) != NULL)
	{
		result += buffer;
	}

	int returnCode = pclose(pipe);
	if (returnCode != 0)
	{
		std::cerr << "Error: Python script execution failed with return code " << returnCode << "." << std::endl;
	}

	return result;
}

int main()
{

	std::cout << executePythonScript("./photo.py");
}