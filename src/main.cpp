/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:32:24 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/18 17:41:06 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../inc/ClientParsing.hpp"
#include "Colors.hpp"
#include "WebServer.hpp"
#include "ServerConfig.hpp"
#include "AdminServer.hpp"

void setSignals(int sig)
{
	std::cout << RESET << std::endl;
	exit(sig);
}

void checkArg(int argc, char **argv, std::vector<std::string> &content)
{
	std::ifstream in;
	std::string name, line;
	if (argc != 2)
		throw std::runtime_error(CYAN "Usage: ./webserv [config file]" RESET);
	else
	{
		name = argv[1];
		if (name.find(".conf") == std::string::npos ||
			name.compare(name.length() - 5, 5, ".conf") ||
			name.find(".conf", name.find(".conf") + 5) != std::string::npos)
			throw std::runtime_error(RED "File is not a configuration file!" RESET);
		in.open(argv[1]);
		if (!in.good())
			throw std::runtime_error(RED "File error!" RESET);
	}
	while (getline(in, line))
		content.push_back(line);
	// 	for (std::vector<std::string>::iterator it = content.begin(); it != content.end(); it++)
	// 	{
	// 		std::cout << "Array line: " << *it << std::endl;
	// 	}
}

std::vector<std::string> checkOneServer(std::vector<std::string> &content, int start, int len)
{
	int brack = 0;
	std::vector<std::string> sCont;
	for (int i = start; i <= start + len; i++)
		sCont.push_back(content[i]);

	// Test for curly brackets
	for (size_t i = 0; i < sCont.size(); i++)
	{
		std::cout << "Server content line: " << sCont[i] << std::endl;
		if (sCont[i].find("{") != std::string::npos)
			brack++;
		if (sCont[i].find("}") != std::string::npos)
			brack--;
	}
	// std::cout << "Brack value: " << brack << std::endl;
	int lNum = sCont[sCont.size() - 1].find("}") != std::string::npos ? start + len + 1 : start + len;
	if (brack > 0)
		parseError("server block not closed at line ", lNum + 1);
	else if (brack < 0)
		parseError("expected block before } at line ", lNum);

	// Test for semicolumn
	for (size_t i = 0; i < sCont.size(); i++)
	{
		size_t l = 0;
		while (sCont[i][l] == ' ' || sCont[i][l] == '\t')
			l++;
		if (sCont[i][l] != '#' && sCont[i][l] != '\0' &&
			(sCont[i].find("server") == std::string::npos ||
			 (sCont[i].find("server") != std::string::npos && sCont[i].find("name") != std::string::npos)) &&
			sCont[i].find("location") == std::string::npos &&
			sCont[i].find("{") == std::string::npos &&
			sCont[i].find("}") == std::string::npos &&
			sCont[i].find(";") == std::string::npos)
			parseError("expected semicolumn at the end of the line ", start + i + 1);
	}
	return sCont;
}

void createServers(std::vector<std::string> &content, std::vector<ServerConfig *> &servers)
{
	int start = 0;
	int count = 1;
	std::vector<std::string> oneServer;
	(void)servers;
	for (size_t i = 0; i < content.size(); i++)
	{
		// std::cout << "Count: " << count << " Start: " << start << " I: " << i << " Line: " << content[i] << std::endl;
		if (content[i].find("server") != std::string::npos && content[i].find("name") == std::string::npos)
		{
			if (count == 1)
				start = i;
			count--;
		}
		if (count == -1 || i == content.size() - 1)
		{
			// std::cout << "Start and i before: " << start << " " << i << std::endl;
			int diff = i == content.size() - 1 ? i - start : i - start - 1;
			// std::cout << "Diff: " << diff << std::endl;
			oneServer = checkOneServer(content, start, diff);
			// createOneServer(servers, oneServer);
			start = i;
			count++;
		}
	}
}

int main(int argc, char **argv)
{
	std::vector<std::string> content;
	std::vector<ServerConfig *> servers;
	try
	{
		signal(SIGINT, &setSignals);
		checkArg(argc, argv, content);
		createServers(content, servers);
		std::cout << "Parsing ok" << std::endl;

		// TODO
		// ServerConfig config("./conf_web/mime.type");	// comprobar que exita el fichero mime.type
		// config.loadConf(argv[1]);

		// AdminServer server(config);
		// server.run();
		// config.print();
	}
	catch (const ServerConfig::ConfErrorException &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
