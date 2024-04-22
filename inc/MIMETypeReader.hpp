/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MIMETypeReader.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 20:00:54 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/22 20:16:43 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIMETYPEREADER_HPP
#define MIMETYPEREADER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
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
