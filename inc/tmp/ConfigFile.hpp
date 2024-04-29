/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:04:28 by vduchi            #+#    #+#             */
/*   Updated: 2024/04/29 15:18:10 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_H__
# define CONFIGFILE_H__

# include "ConfigServer.hpp"

class ConfigFile
{
	private:
		ConfigFile();
		std::map<int, ConfigServer *> _srvs;
		std::map<std::string, std::string> _gen;

	public:
		ConfigFile(const std::string &);
		~ConfigFile();
	
		class ConfErrorException : std::runtime_error
		{
			public:
				ConfErrorException(const std::string &);
		};
		typedef ConfigFile::ConfErrorException e_cee;
};

#endif