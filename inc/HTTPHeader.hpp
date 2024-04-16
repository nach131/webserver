/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPHeader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 17:32:08 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/16 19:12:06 by nmota-bu         ###   ########.fr       */
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

public:
    HTTPHeader(const std::string &prot, const std::string &code)
    {

        std::cout << YELLOW << prot << std::endl;
        std::cout << YELLOW << code << std::endl;

        std::string result;

        for (size_t i = 0; i < prot.length(); ++i)
        {
            char character = prot[i]; // Obtener el carácter en la posición i
            result += character;
            std::cout << character << std::endl; // Imprimir el carácter
        }
        result += " ";
        for (size_t i = 0; i < code.length(); ++i)
        {
            char character = code[i]; // Obtener el carácter en la posición i
            result += character;
            std::cout << character << std::endl; // Imprimir el carácter
        }

        std::cout << MAGENTA << result << std::endl;
    }

    // Método para agregar un campo al encabezado HTTP
    void addField(const std::string &key, const std::string &value)
    {
        _fields[key] = value;
    }

    // Método para obtener el encabezado HTTP como una cadena
    std::string toString() const
    {

        std::string header;

        for (FieldMap::const_iterator it = _fields.begin(); it != _fields.end(); ++it)
        {
            header += it->first + ": " + it->second + "\r\n";
        }
        header += "\r\n";
        return header;
    }
};

#endif
