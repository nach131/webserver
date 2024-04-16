#include <iostream>
#include <string>
#include <map>

class HTTPHeader {
private:
    typedef std::map<std::string, std::string> FieldMap;
    FieldMap fields;

public:
    // Constructor que toma la respuesta del cliente como argumento
    HTTPHeader(const std::string& clientResponse) {
        parseClientResponse(clientResponse);
    }

    // Método para agregar un campo al encabezado HTTP
    void addField(const std::string& key, const std::string& value) {
        fields[key] = value;
    }

    // Método para obtener el encabezado HTTP como una cadena
    std::string toString() const {
        std::string header;
        for (FieldMap::const_iterator it = fields.begin(); it != fields.end(); ++it) {
            header += it->first + ": " + it->second + "\r\n";
        }
        header += "\r\n";
        return header;
    }

private:
    // Método privado para analizar la respuesta del cliente y extraer los campos relevantes
    void parseClientResponse(const std::string& clientResponse) {
        std::string::size_type pos = 0;
        std::string::size_type prevPos = 0;
        while ((pos = clientResponse.find("\r\n", prevPos)) != std::string::npos) {
            std::string line = clientResponse.substr(prevPos, pos - prevPos);
            prevPos = pos + 2; // Avanzar la posición inicial para la próxima iteración
            std::string::size_type colonPos = line.find(": ");
            if (colonPos != std::string::npos) {
                std::string key = line.substr(0, colonPos);
                std::string value = line.substr(colonPos + 2);
                addField(key, value);
            }
        }
    }
};

int main() {
    std::string clientResponse = "GET /style.css HTTP/1.1\r\n"
                                 "Host: localhost:8080\r\n"
                                 "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:124.0) Gecko/20100101 Firefox/124.0\r\n"
                                 "Accept: text/css,*/*;q=0.1\r\n"
                                 "Accept-Language: es-ES,es;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
                                 "Accept-Encoding: gzip, deflate, br\r\n"
                                 "Connection: keep-alive\r\n"
                                 "Referer: http://localhost:8080/\r\n"
                                 "Sec-Fetch-Dest: style\r\n"
                                 "Sec-Fetch-Mode: no-cors\r\n"
                                 "Sec-Fetch-Site: same-origin\r\n";

    // Crear objeto HTTPHeader y pasar la respuesta del cliente como argumento
    HTTPHeader header(clientResponse);

    // Agregar otros campos al encabezado si es necesario
    header.addField("Content-Type", "text/css");
    // Supongamos que tienes una longitud de contenido disponible
    header.addField("Content-Length", "100");

    // Obtener el encabezado HTTP como una cadena y mostrarlo
    std::string headerStr = header.toString();
    std::cout << "Header:\n" << headerStr << std::endl;

    return 0;
}
