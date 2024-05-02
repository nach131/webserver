#include <iostream>
#include <string>

bool isFile(const std::string& path) {
    std::size_t found = path.find_last_of(".");
    return (found != std::string::npos && found != path.size() - 1);
}

int main() {
    std::string path1 = "archivo.txt";
    std::string path2 = "carpeta/archivo";
    std::string path3 = "carpeta/archivo.";
    std::string path4 = "carpeta/archivo.jpg";
    
    std::cout << path1 << " tiene extensión: " << std::boolalpha << isFile(path1) << std::endl;
    std::cout << path2 << " tiene extensión: " << std::boolalpha << isFile(path2) << std::endl;
    std::cout << path3 << " tiene extensión: " << std::boolalpha << isFile(path3) << std::endl;
    std::cout << path4 << " tiene extensión: " << std::boolalpha << isFile(path4) << std::endl;
    
    return 0;
}

// archivo.txt tiene extensión: true
// carpeta/archivo tiene extensión: false
// carpeta/archivo. tiene extensión: false
// carpeta/archivo.jpg tiene extensión: true