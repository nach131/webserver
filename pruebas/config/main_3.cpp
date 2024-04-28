#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>

class ServerConfiguration {
private:
    int port;
    std::string serverName;
    std::string rootDirectory;
    std::string errorPage;
    std::string apiForward;
    int apiPort;
    std::map<std::string, std::map<std::string, std::set<std::string> > > locations; // Mapa de ubicaciones con sus configuraciones

public:
    ServerConfiguration() : port(8080), apiPort(0) {} // Valores por defecto

    void loadConfiguration(const std::string& filename) {
        std::ifstream configFile(filename.c_str());
        std::string line;
        std::string location;

        while (std::getline(configFile, line)) {
            if (line.find("server {") != std::string::npos) {
                while (std::getline(configFile, line)) {
                    if (line.find("}") != std::string::npos)
                        break;
                    std::istringstream iss(line);
                    std::string key, value;
                    iss >> key >> value;
                    if (key == "listen") {
                        port = std::stoi(value);
                    } else if (key == "server_name") {
                        serverName = value;
                    } else if (key == "root") {
                        rootDirectory = value;
                    } else if (key == "error_page") {
                        errorPage = value;
                    } else if (key == "api_forward") {
                        apiForward = value;
                    } else if (key == "api_port") {
                        apiPort = std::stoi(value);
                    } else if (key == "location") {
                        location = value;
                        locations[location] = std::map<std::string, std::set<std::string> >();
                        while (std::getline(configFile, line)) {
                            if (line.find("}") != std::string::npos)
                                break;
                            std::istringstream iss2(line);
                            std::string key2, value2;
                            iss2 >> key2 >> value2;
                            if (key2 == "allow_methods") {
                                std::istringstream iss3(value2);
                                std::string method;
                                while (iss3 >> method) {
                                    locations[location][key2].insert(method);
                                }
                            } else {
                                locations[location][key2].insert(value2);
                            }
                        }
                    }
                }
            }
        }
    }

    void printConfiguration() const {
        std::cout << "Server Configuration:" << std::endl;
        std::cout << "Port: " << port << std::endl;
        std::cout << "Server Name: " << serverName << std::endl;
        std::cout << "Root Directory: " << rootDirectory << std::endl;
        std::cout << "Error Page: " << errorPage << std::endl;
        std::cout << "API Forward: " << apiForward << std::endl;
        std::cout << "API Port: " << apiPort << std::endl;

        std::cout << "Locations:" << std::endl;
        for (std::map<std::string, std::map<std::string, std::set<std::string> > >::const_iterator it = locations.begin(); it != locations.end(); ++it) {
            std::cout << "  " << it->first << ":" << std::endl;
            for (std::map<std::string, std::set<std::string> >::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
                std::cout << "    " << it2->first << ": ";
                for (std::set<std::string>::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3) {
                    std::cout << *it3 << " ";
                }
                std::cout << std::endl;
            }
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <nombre_archivo_configuracion>" << std::endl;
        return 1;
    }

    ServerConfiguration serverConfig;
    serverConfig.loadConfiguration(argv[1]);
    serverConfig.printConfiguration();

    // Aquí iría el código para iniciar el servidor HTTP con la configuración cargada

    return 0;
}
