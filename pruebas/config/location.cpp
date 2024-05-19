#include <iostream>
#include <map>
#include <string>
#include <algorithm>

// Aquí está el mapa de configuraciones
std::map<std::string, std::map<std::string, std::string> > _locations;

// Inicializa el mapa de configuraciones
void initializeLocations() {
    // root
    _locations["/"]["autoindex"] = "off";
    _locations["/"]["root"] = "./dist";
    _locations["/"]["allow_methods"] = "DELETE POST GET";
    // files
    _locations["/files"]["autoindex"] = "on";
    _locations["/files"]["root"] = "./dist";
    _locations["/files"]["allow_methods"] = "GET";
    _locations["/files"]["index"] = "files.html";
    // other files
    _locations["/other_files"]["autoindex"] = "on";
    _locations["/other_files"]["root"] = "./dist";
    _locations["/other_files"]["allow_methods"] = "GET";
    // colors
    _locations["/colors"]["autoindex"] = "off";
    _locations["/colors"]["root"] = "./dist";
    _locations["/colors"]["allow_methods"] = "GET";
    _locations["/colors"]["index"] = "colors.html";
    // file on location
    _locations["/astronaut/toma.html"]["autoindex"] = "off";
    _locations["/astronaut/toma.html"]["root"] = "./dist/_TEST";
    _locations["/astronaut/toma.html"]["allow_methods"] = "GET";
    // Alias "redirecred URL" tine que buscar index.html
    _locations["/tomate/mas/otra/cosa.html"]["autoindex"] = "off";
    _locations["/tomate/mas/otra/cosa.html"]["alias"] = "./dist/files";
    _locations["/tomate/mas/otra/cosa.html"]["allow_methods"] = "GET";
    // cgi
    _locations["/cgi_bin"]["autoindex"] = "off";
    _locations["/cgi_bin"]["root"] = "/Users/nmota-bu/Desktop/webserver";
    _locations["/cgi_bin"]["allow_methods"] = "GET";
    _locations["/cgi_bin"]["index"] = "photo.py";
    // TEST 403 Forbidden
    _locations["/_TEST"]["autoindex"] = "off";
    _locations["/_TEST"]["root"] = "./dist";
    _locations["/_TEST"]["allow_methods"] = "GET";
    // EARTH
    _locations["/earth"]["autoindex"] = "off";
    _locations["/earth"]["root"] = "./dist_1";
    _locations["/earth"]["allow_methods"] = "GET";
    // NO ALLOWED
    _locations["/nada"]["allow_methods"] = "";
}

std::map<std::string, std::string> getLocationConfig(const std::string& path) {
    std::string best_match = "/";
    std::map<std::string, std::map<std::string, std::string> >::iterator it;
    for (it = _locations.begin(); it != _locations.end(); ++it) {
        if (path.find(it->first) == 0 && it->first.length() > best_match.length()) {
            best_match = it->first;
        }
    }
    return _locations[best_match];
}

int main() {
    initializeLocations();

    std::string path = "/colors/toma/form.html";
    std::map<std::string, std::string> config = getLocationConfig(path);

    std::cout << "Configuration for " << path << ":\n";
    std::map<std::string, std::string>::iterator it;
    for (it = config.begin(); it != config.end(); ++it) {
        std::cout << it->first << ": " << it->second << '\n';
    }

    return 0;
}
