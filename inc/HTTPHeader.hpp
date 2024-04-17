/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPHeader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 17:32:08 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/17 14:31:53 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHEADER_HPP
#define HTTPHEADER_HPP

#include <iostream>
#include <string>
#include <map>

//========================PARSEANDO BUFFER==========================================

// class HTTPHeader
// {
// private:
//     typedef std::map<std::string, std::string> FieldMap;
//     FieldMap fields;

// public:
//     // Constructor que toma la respuesta del cliente como argumento
//     HTTPHeader(const std::string &clientResponse)
//     {
//         parseClientResponse(clientResponse);
//     }

//     // Método para agregar un campo al encabezado HTTP
//     void addField(const std::string &key, const std::string &value)
//     {
//         fields[key] = value;
//     }

//     // Método para obtener el encabezado HTTP como una cadena
//     std::string getHeader() const
//     {
//         std::string header;
//         for (FieldMap::const_iterator it = fields.begin(); it != fields.end(); ++it)
//         {
//             header += it->first + ": " + it->second + "\r\n";
//         }
//         header += "\r\n";
//         return header;
//     }

// private:
//     // Método privado para analizar la respuesta del cliente y extraer los campos relevantes
//     void parseClientResponse(const std::string &clientResponse)
//     {
//         std::string::size_type pos = 0;
//         std::string::size_type prevPos = 0;
//         while ((pos = clientResponse.find("\r\n", prevPos)) != std::string::npos)
//         {
//             std::string line = clientResponse.substr(prevPos, pos - prevPos);
//             prevPos = pos + 2; // Avanzar la posición inicial para la próxima iteración
//             std::string::size_type colonPos = line.find(": ");
//             if (colonPos != std::string::npos)
//             {
//                 std::string key = line.substr(0, colonPos);
//                 std::string value = line.substr(colonPos + 2);
//                 addField(key, value);
//             }
//         }
//     }
// };
//========================MAP=========================================

// class HTTPHeader
// {
// private:
//     typedef std::map<std::string, std::string> FieldMap;
//     FieldMap _fields;
//     std::string _header;

// public:
//     HTTPHeader(const std::string &prot, const std::string &code)
//     {
//         std::string tmp = prot + " " + code + "\r\n";
//         _header = tmp;
//     }

//     // Método para agregar un campo al encabezado HTTP
//     void addField(const std::string &key, const std::string &value)
//     {
//         _fields[key] = value;
//     }

//     // Método para obtener el encabezado HTTP como una cadena
//     std::string getHeader() const
//     {
//         std::string header = _header;

//         for (FieldMap::const_iterator it = _fields.begin(); it != _fields.end(); ++it)
//             header += it->first + ": " + it->second + "\r\n";
//         header += "\r\n";
//         return header;
//     }

//     size_t headerLength() const { return _header.length(); }
// };

//========================string=========================================

class HTTPHeader
{
private:
    std::string _header;

public:
    HTTPHeader(const std::string &prot, const std::string &code)
    {
        _header = prot + " " + code + "\r\n";
    }

    // Método para agregar un campo al encabezado HTTP
    void addField(const std::string &key, const std::string &value)
    {
        _header += key + ": " + value + "\r\n";
    }

    // Método para obtener el encabezado HTTP como una cadena
    std::string getHeader() const
    {
        std::string header = _header + "\r\n";
        return header;
    }

    size_t headerLength() const { return _header.length(); }
};

#endif

//===========================PARA POST==============================================

// <form action="/" method="post" enctype="multipart/form-data">
//   <input type="text" name="description" value="some text" />
//   <input type="file" name="myFile" />
//   <button type="submit">Submit</button>
// </form>

// POST /foo HTTP/1.1
// Content-Length: 68137
// Content-Type: multipart/form-data; boundary=---------------------------974767299852498929531610575

// ---------------------------974767299852498929531610575
// Content-Disposition: form-data; name="description"

// some text
// ---------------------------974767299852498929531610575
// Content-Disposition: form-data; name="myFile"; filename="foo.txt"
// Content-Type: text/plain

// (content of the uploaded file foo.txt)
// ---------------------------974767299852498929531610575
