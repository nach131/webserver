#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>

// Función para manejar las solicitudes CGI en PHP
void handlePHPCGI(const std::string& path, const std::string& postData) {
    std::string command = "/usr/bin/php-cgi " + path; // Ruta al intérprete de PHP CGI
    FILE* pipe = popen(command.c_str(), "w");
    if (pipe) {
        // Si es una solicitud POST, envía los datos al script PHP
        if (!postData.empty()) {
            fputs(postData.c_str(), pipe);
        }
        pclose(pipe);
    } else {
        std::cerr << "Error: No se pudo abrir el proceso para el script PHP" << std::endl;
    }
}

// Función para manejar las solicitudes GET y POST
void handleRequest(const std::string& method, const std::string& path, const std::string& postData) {
    // Si la solicitud es un script CGI en PHP, maneja el script
    if (path.find(".php") != std::string::npos) {
        handlePHPCGI(path, postData);
    } else {
        // Si la solicitud es un archivo estático, como HTML, CSS o JavaScript, envía el archivo
        std::ifstream file(path.c_str());
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::cout << "HTTP/1.1 200 OK\r\n";
            std::cout << "Content-Length: " << buffer.str().size() << "\r\n";
            std::cout << "\r\n";
            std::cout << buffer.str();
        } else {
            // Si el archivo no se puede abrir, devuelve un error 404
            std::cerr << "Error: Archivo no encontrado: " << path << std::endl;
            std::cout << "HTTP/1.1 404 Not Found\r\n";
            std::cout << "Content-Length: 0\r\n";
            std::cout << "\r\n";
        }
    }
}

int main() {
    // Simulamos una solicitud GET para probar
    std::string method = "GET";
    std::string path = "/index.php";
    std::string postData = "";

    // Maneja la solicitud
    handleRequest(method, path, postData);

    // Simulamos una solicitud POST para probar
    method = "POST";
    path = "/process.php";
    postData = "name=John&age=30";

    // Maneja la solicitud
    handleRequest(method, path, postData);

    return 0;
}
