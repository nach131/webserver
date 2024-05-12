/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 15:05:34 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/12 23:08:32 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"

CGIHandler::CGIHandler() {} // No se utiliza

CGIHandler::CGIHandler(int kq) : _kq(kq)
{
	FD_ZERO(&_fdSet);
}

CGIHandler::~CGIHandler() {}

void CGIHandler::runCGI()
{
	int p[2], status;
	pid_t pid;

	pipe(p);
	pid = fork();
	if (pid == 0)
	{
		// Hijo
	}
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		// TODO Controlar las senales
		// WTERMSIG te devuelve el numero de la senal
	}
	else if (WIFEXITED(status))
	{
		// Si todo ha ido bien
	}
}

bool CGIHandler::checkFD(int fd) { return FD_ISSET(fd, &_fdSet); }

void CGIHandler::assignFD(int fd) { FD_SET(fd, &_fdSet); }