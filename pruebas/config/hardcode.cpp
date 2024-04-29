#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

class ServerConfig
{
private:
    int _port;
    std::string _serverName;
    std::string _rootDirectory;
    std::map<int, std::string> _errorPages; // Mapa de códigos de error y sus archivos asociados
    std::string _apiForward;
    int _apiPort;
    std::map<std::string, std::map<std::string, std::string> >  _locations; // Mapa de ubicaciones con sus configuraciones

public:
    ServerConfig() : _port(8080), _apiPort(3000), _apiForward("192.168.1.20") {} // Valores por defecto

    void loadConf(const std::string &filename)
    {
        (void)filename;
        _serverName = "locahost";
        _rootDirectory = "dist/";

        _errorPages[404] = "config_web/error/404.html";
        _errorPages[405] = "config_web/error/405.html";

        _locations["/"] = std::map<std::string, std::string>();
            _locations["/"]["autoindex"] = "off";
            _locations["/"]["allow_methods"] = "DELETE POST GET";
        _locations["/files"] = std::map<std::string, std::string>();
            _locations["/files"]["autoindex"] = "on";
            _locations["/files"]["allow_methods"] = "GET";
        _locations["/cgi_bin"] = std::map<std::string, std::string>();
            _locations["/cgi_bin"]["autoindex"] = "on";
            _locations["/cgi_bin"]["allow_methods"] = "GET";
            _locations["/cgi_bin"]["root"] = "./";
            _locations["/cgi_bin"]["index"] = "calc.py";
            _locations["/cgi_bin"]["cgi_path"] = "/usr/bin/python3 /bin/bash";
            _locations["/cgi_bin"]["cgi_ext"] = "cgi_ext .py .sh";
    }

    void print() const
    {
        std::cout << "Server Configuration:" << std::endl;
        std::cout << "Port: " << _port << std::endl;
        std::cout << "Server Name: " << _serverName << std::endl;
        std::cout << "Root Directory: " << _rootDirectory << std::endl;
        std::cout << "Error Pages:" << std::endl;
        for (std::map<int, std::string>::const_iterator it = _errorPages.begin(); it != _errorPages.end(); ++it)
            std::cout << "  " << it->first << ": " << it->second << std::endl;
        std::cout << "API Forward: " << _apiForward << std::endl;
        std::cout << "API Port: " << _apiPort << std::endl;

        std::cout << "Locations:" << std::endl;
        for (std::map<std::string, std::map<std::string, std::string> >::const_iterator it = _locations.begin(); it != _locations.end(); ++it)
        {
            std::cout << "  " << it->first << ":" << std::endl;
            for (std::map<std::string, std::string>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                std::cout << "    " << it2->first << ": " << it2->second << std::endl;
        }
    }

    int getPort() const { return _port; }

    std::string getServerName() const { return _serverName; }

    std::string getRootDirectory() const { return _rootDirectory; }

    std::string getErrorPage(int errorCode) const
    {
        std::map<int, std::string>::const_iterator it = _errorPages.find(errorCode);
        if (it != _errorPages.end())
            return it->second;
        else
            return ""; // Opción por defecto si no se encuentra la página de error
    }

    std::string getApiForward() const { return _apiForward; }

    int getApiPort() const { return _apiPort; }

    std::map<std::string, std::map<std::string, std::string> > getLocation() { return _locations;}
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Uso: " << argv[0] << " <nombre_archivo_configuracion>" << std::endl;
        return 1;
    }

    ServerConfig config;
    config.loadConf(argv[1]);
    // config.print();

    std::cout << config.getErrorPage(404) << std::endl;
    std::map<std::string, std::map<std::string, std::string> > locations;

    locations = config.getLocation();

    for (std::map<std::string, std::map<std::string, std::string> >::const_iterator it = locations.begin(); it != locations.end(); ++it)
        {
            std::cout << "  " << it->first << ":" << std::endl;
            for (std::map<std::string, std::string>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                std::cout << "    " << it2->first << ": " << it2->second << std::endl;
        }




    return 0;
}


// Server Configuration:
// Port: 8080
// Server Name: locahost
// Root Directory: dist/
// Error Pages:
//   404: config_web/error/404.html
//   405: config_web/error/405.html
// API Forward: 192.168.1.20
// API Port: 3000
// Locations:
//   /:
//     allow_methods: DELETE POST GET
//     autoindex: off
//   /cgi_bin:
//     allow_methods: GET
//     autoindex: on
//     cgi_ext: cgi_ext .py .sh
//     cgi_path: /usr/bin/python3 /bin/bash
//     index: calc.py
//     root: ./
//   /files:
//     allow_methods: GET
//     autoindex: on