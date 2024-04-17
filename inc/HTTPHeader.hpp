/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPHeader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 17:32:08 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/17 12:30:33 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHEADER_HPP
#define HTTPHEADER_HPP

#include <iostream>
#include <string>
#include <map>

class HTTPHeader
{
private:
    typedef std::map<std::string, std::string> FieldMap;
    FieldMap _fields;
    std::string _header;

public:
    HTTPHeader(const std::string &prot, const std::string &code);
    ~HTTPHeader();

    std::string DateField() const;

    // Método para agregar un campo al encabezado HTTP
    void addField(const std::string &key, const std::string &value);

    // Método para obtener el encabezado HTTP como una cadena
    std::string getHeader() const;

    size_t headerLength() const;
};

#endif
