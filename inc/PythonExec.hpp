/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PythonExec.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 10:02:51 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/28 17:11:47 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PYTHONEXEC_HPP
#define PYTHONEXEC_HPP

#include <iostream>
#include <WebServer.hpp>
#include "Colors.hpp"

std::string execPython(const std::string &py);
std::string execPython(const std::string &py, const std::string &data);
std::string execPython(const std::string &py, const std::string &dirPath, const std::string &root_location);

#endif
