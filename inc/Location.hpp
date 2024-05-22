/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 17:46:28 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/22 11:45:55 by nmota-bu         ###   ########.fr       */
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
	std::string const &getAlias() const;
	std::string const &getIndex() const;

public:
	Location();
	~Location();
	void init(const LocationResult &location, const std::string &path, const std::string &referer);
	void init(const LocationResult &location, const std::string &path);

	bool autoIndexOn();
	bool methodAllowed(const std::string &method);
	std::string const &getRoot() const;
	std::string const &realPath() const;
	std::string const &getName() const;

	void print();
};

#endif
