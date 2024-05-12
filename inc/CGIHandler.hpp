/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 15:05:34 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/12 23:08:35 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_H__
#define CGIHANDLER_H__

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
// #include <sys/select.h>

class CGIHandler
{
private:
	int _kq;
	fd_set _fdSet;
	CGIHandler();

public:
	CGIHandler(int);
	~CGIHandler();

	void runCGI();
	bool checkFD(int);
	void assignFD(int);
};

#endif