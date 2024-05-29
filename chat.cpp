#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 4096

// Function to write the received data to a file
void writeToFile(const std::string &filename, const std::string &data) {
    std::ofstream file(filename.c_str(), std::ios::binary);
    if (file.is_open()) {
        file.write(data.c_str(), data.size());
        file.close();
        std::cout << "File saved: " << filename << std::endl;
    } else {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
    }
}

// Function to parse multipart/form-data and save the file
void parseMultipartFormData(const std::string &boundary, const std::string &content) {
    size_t pos = 0;
    std::string delimiter = "--" + boundary;
    std::string endDelimiter = delimiter + "--";

    while ((pos = content.find(delimiter, pos)) != std::string::npos) {
        size_t nextPos = content.find(delimiter, pos + delimiter.length());
        if (nextPos == std::string::npos) break;

        std::string part = content.substr(pos + delimiter.length(), nextPos - pos - delimiter.length());
        pos = nextPos;

        // Parse headers
        size_t headerEnd = part.find("\r\n\r\n");
        if (headerEnd == std::string::npos) continue;

        std::string headers = part.substr(0, headerEnd);
        std::string body = part.substr(headerEnd + 4);

        // Find the filename
        size_t filenamePos = headers.find("filename=\"");
        if (filenamePos == std::string::npos) continue;
        filenamePos += 10; // Length of 'filename="'

        size_t filenameEnd = headers.find("\"", filenamePos);
        if (filenameEnd == std::string::npos) continue;

        std::string filename = headers.substr(filenamePos, filenameEnd - filenamePos);

        // Write the file
        writeToFile(filename, body);
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Waiting for a connection..." << std::endl;

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    std::stringstream requestStream;
    int bytesReceived;

    // Read the request
    while ((bytesReceived = recv(new_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        requestStream.write(buffer, bytesReceived);
    }

    std::string request = requestStream.str();
    size_t boundaryPos = request.find("boundary=");
    if (boundaryPos == std::string::npos) {
        std::cerr << "Boundary not found in the request" << std::endl;
        close(new_socket);
        close(server_fd);
        return 1;
    }

    // Extract boundary value
    size_t boundaryEnd = request.find("\r\n", boundaryPos);
    std::string boundary = request.substr(boundaryPos + 9, boundaryEnd - boundaryPos - 9);

    // Extract the content after the headers
    size_t contentStart = request.find("\r\n\r\n");
    if (contentStart == std::string::npos) {
        std::cerr << "Content not found in the request" << std::endl;
        close(new_socket);
        close(server_fd);
        return 1;
    }

    std::string content = request.substr(contentStart + 4);

    // Parse and save the multipart/form-data content
    parseMultipartFormData(boundary, content);

    close(new_socket);
    close(server_fd);
    return 0;
}
