/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelona>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 20:29:26 by vduchi            #+#    #+#             */
/*   Updated: 2024/05/02 21:47:06 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/tmp/ConfigServer.hpp"

ConfigServer::ConfigServer(const std::vector<std::string> & arr, size_t & i)
{
	int brack = 1;

	for (; i < arr.size() && brack != 0; i++)
	{
		std::string key, val;
		std::stringstream ss(arr[i]);
		ss >> key;
		// TODO: Controlar que la key este' bien
		while (ss >> val)
		{
			// TODO: Controlar que los valores esten bien
		}
	}
}
