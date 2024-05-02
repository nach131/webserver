#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>

class ServerConfiguration
{
private:
    int _port;
    std::string _serverName;
    std::string _rootDirectory;
    std::map<int, std::string> _errorPages; // Mapa de códigos de error y sus archivos asociados
    std::string _apiForward;
    int _apiPort;
    std::map<std::string, std::map<std::string, std::string>> _locations; // Mapa de ubicaciones con sus configuraciones

public:
    ServerConfiguration() : _port(8080), _apiPort(3000), _apiForward("192.168.1.20") {} // Valores por defecto

    void loadConfiguration(const std::string &filename)
    {
        std::ifstream configFile(filename.c_str());
        std::string line;
        std::string location;

        while (std::getline(configFile, line))
        {
            if (line.find("server {") != std::string::npos)
            {
                while (std::getline(configFile, line))
                {
                    if (line.find("}") != std::string::npos)
                        break;
                    std::istringstream iss(line);
                    std::string key, value;
                    iss >> key >> value;
					std::cout << "Key: -" << key << "- Value: -" << value << "-" << std::endl;
                    if (key == "listen")
                        _port = std::stoi(value);
                    else if (key == "server_name")
                        _serverName = value;
                    else if (key == "root")
                        _rootDirectory = value;
                    else if (key == "error_page")
                    {
                        int errorCode;
                        std::string errorFile;
                        iss >> errorFile;
						errorCode = atoi(value.c_str());
						std::cout << "Code: " << errorCode << " and File:" << errorFile << std::endl;
                        _errorPages[errorCode] = errorFile;
                    }
                    else if (key == "api_forward")
                        _apiForward = value;
                    else if (key == "api_port")
                        _apiPort = std::stoi(value);
                    else if (key == "location")
                    {
                        location = value;
                        _locations[location] = std::map<std::string, std::string>();
                        while (std::getline(configFile, line))
                        {
                            if (line.find("}") != std::string::npos)
                                break;
                            std::istringstream iss2(line);
                            std::string key2, value2;
                            iss2 >> key2 >> value2;
                            if (key2 == "allow_methods")
                                _locations[location][key2] += value2 + " ";
                            else
                                _locations[location][key2] = value2;
                        }
                    }
                }
            }
        }
		std::cout << std::endl;
    }

    void printConfiguration() const
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
        for (std::map<std::string, std::map<std::string, std::string>>::const_iterator it = _locations.begin(); it != _locations.end(); ++it)
        {
            std::cout << "  " << it->first << ":" << std::endl;
            for (std::map<std::string, std::string>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                std::cout << "    " << it2->first << ": " << it2->second << std::endl;
        }
    }
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Uso: " << argv[0] << " <nombre_archivo_configuracion>" << std::endl;
        return 1;
    }

    ServerConfiguration serverConfig;
    serverConfig.loadConfiguration(argv[1]);
    serverConfig.printConfiguration();

    // Aquí iría el código para iniciar el servidor HTTP con la configuración cargada

    return 0;
}
