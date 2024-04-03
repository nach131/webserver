/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelona.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:04:28 by vduchi            #+#    #+#             */
/*   Updated: 2024/03/12 16:10:11 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_H__
# define CONFIGFILE_H__

# include "WebServer.hpp"
# include "ConfigServer.hpp"

class ConfigFile
{
	private:
		std::map<int, ConfigServer *> _conf;
};

#endif
