#include <iostream>
#include <string>
#include <random>
#include <sstream>
#include <regex>

std::string generateToken(size_t length)
{
    const char hex_chars[] = "0123456789abcdef";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);

    std::stringstream ss;
    for (size_t i = 0; i < length; ++i)
    {
        ss << hex_chars[dis(gen)];
    }
    return ss.str();
}

bool isValidToken(const std::string &token, size_t expectedLength)
{
    // Verificar la longitud del token
    if (token.length() != expectedLength)
    {
        return false;
    }

    // Verificar si el token contiene solo caracteres hexadecimales
    const std::regex hexPattern("^[0-9a-f]+$");
    if (!std::regex_match(token, hexPattern))
    {
        return false;
    }

    return true;
}

int main()
{
    size_t length = 32;                        // Longitud del token
    std::string token = generateToken(length); // Generar un token de ejemplo
    std::cout << "Generated Token: " << token << std::endl;

    std::string toma = "27a64cc3236abfdb0b520f977e005268";

    // Comprobar si el token es válido
    if (isValidToken(token, length))
        std::cout << "The token is valid." << std::endl;
    else
        std::cout << "The token is invalid." << std::endl;

    if (isValidToken(toma, length))
        std::cout << "The token is valid." << std::endl;
    else
        std::cout << "The token is invalid." << std::endl;

    return 0;
}
