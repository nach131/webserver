/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 17:46:28 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/23 16:52:01 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <map>
#include <sstream>
#include "WebServer.hpp"

struct LocationResult
{
	std::string name;
	std::string realPath;
	std::map<std::string, std::string> config;
};

class Location
{
private:
	LocationResult _location;
	std::string const &getIndex() const;

public:
	Location();
	~Location();
	void init_no(const LocationResult &location, const std::string &path, const std::string &referer);
	void init(const LocationResult &location, const std::string &path, const std::string &referer);

	bool autoIndexOn();
	bool methodAllowed(const std::string &method);
	bool isFileLocation();

	std::string const &getAlias() const;
	std::string const &getRoot() const;
	std::string const &realPath() const;
	std::string const &getName() const;

	void print();
};

LocationResult compareLocationResults(const LocationResult &loc1, const LocationResult &loc2);

#endif
