/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPHeader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmota-bu <nmota-bu@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 17:32:08 by nmota-bu          #+#    #+#             */
/*   Updated: 2024/04/22 15:56:49 by nmota-bu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHEADER_HPP
#define HTTPHEADER_HPP

#include <iostream>
#include <string>
#include <map>

//========================string=========================================

class HTTPHeader
{
private:
    std::string _header;

public:
    HTTPHeader();
    ~HTTPHeader();

    HTTPHeader(const std::string &prot, const std::string &code);

    // Método para agregar un campo al encabezado HTTP
    void addOne(const std::string &prot, const std::string &code);

    // Método para agregar un campo al encabezado HTTP
    void addField(const std::string &key, const std::string &value);
    // Método para obtener el encabezado HTTP como una cadena

    std::string const getHeader() const;

    size_t headerLength() const;
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
