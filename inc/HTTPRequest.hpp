#include <iostream>
#include <string>
#include <map>

class HTTPRequest
{
private:
    std::string _method;
    std::string _path;
    std::string _version;
    std::map<std::string, std::string> _headers;
    std::string _body;

public:
    HTTPRequest(const std::string &request)
    {
        // Parse the request line
        size_t pos = request.find(' ');
        _method = request.substr(0, pos);
        size_t next_pos = request.find(' ', pos + 1);
        _path = request.substr(pos + 1, next_pos - pos - 1);
        pos = next_pos;
        next_pos = request.find('\r', pos);
        _version = request.substr(pos + 1, next_pos - pos - 1);

        // Parse headers
        pos = request.find('\n', next_pos + 2);
        while (pos != std::string::npos && request[pos] != '\r')
        {
            next_pos = request.find(':', pos);
            std::string key = request.substr(pos + 2, next_pos - pos - 2);
            pos = request.find('\n', next_pos);
            next_pos = request.find('\r', pos);
            std::string value = request.substr(pos + 2, next_pos - pos - 2);
            _headers[key] = value;
            pos = request.find('\n', next_pos + 2);
        }

        // Parse body
        pos = request.find("\r\n\r\n") + 4;
        _body = request.substr(pos);
    }

    std::string getMethod() const { return _method; }

    std::string getPath() const { return _path; }

    std::string getVersion() const { return _version; }

    std::string getHeader(const std::string &key) const
    {
        std::map<std::string, std::string>::const_iterator it = _headers.find(key);
        if (it != _headers.end())
            return it->second;
        else
            return "";
    }

    std::string getBody() const { return _body; }
};

// int main()
// {
//     std::string request = "POST /api/users HTTP/1.1\r\n"
//                           "Host: localhost:8080\r\n"
//                           "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:125.0) Gecko/20100101 Firefox/125.0\r\n"
//                           "Accept: */*\r\n"
//                           "Accept-Language: es-ES,es;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
//                           "Accept-Encoding: gzip, deflate, br\r\n"
//                           "Content-Type: application/json\r\n"
//                           "Content-Length: 72\r\n"
//                           "Origin: http://localhost:8080\r\n"
//                           "Connection: keep-alive\r\n"
//                           "Referer: http://localhost:8080/form.html\r\n"
//                           "Sec-Fetch-Dest: empty\r\n"
//                           "Sec-Fetch-Mode: cors\r\n"
//                           "Sec-Fetch-Site: same-origin\r\n\r\n"
//                           "{\"username\":\"pablo\",\"full_name\":\"pablo marlmol\",\"email\":\"pablo@toma.es\"}";

//     HTTPRequest httpRequest(request);

//     std::cout << "Method: " << httpRequest.getMethod() << std::endl;
//     std::cout << "Path: " << httpRequest.getPath() << std::endl;
//     std::cout << "Version: " << httpRequest.getVersion() << std::endl;
//     std::cout << "Content-Type: " << httpRequest.getHeader("Content-Type") << std::endl;
//     std::cout << "Content-Length: " << httpRequest.getHeader("Content-Length") << std::endl;
//     std::cout << "Body: " << httpRequest.getBody() << std::endl;

//     return 0;
// }
