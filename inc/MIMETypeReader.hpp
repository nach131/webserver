/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MIMETypeReader.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduchi <vduchi@student.42barcelon>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 20:00:54 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/05/19 16:10:42 by vduchi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIMETYPEREADER_HPP
#define MIMETYPEREADER_HPP

#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

class MIMETypeReader
{
private:
	std::map<std::string, std::string> _mimeTypes;

public:
	MIMETypeReader(const std::string &filePath);
	~MIMETypeReader();
	std::string getContentType(const std::string &extension) const;
	void print();

private:
	void loadMIMETypes(const std::string &filePath);
};

#endif
