#include <iostream>
#include <fstream>
#include <string>

void writeToFile(const std::string &filename, const std::string &data, bool create)
{
    std::ofstream file;

    if (create)
        // Abre el archivo en modo de creación (truncado)
        file.open(filename.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
    else
        // Abre el archivo en modo de añadir
        file.open(filename.c_str(), std::ios::out | std::ios::binary | std::ios::app);

    if (!file.is_open())
        std::cerr << "Error: Unable to open file for writing." << std::endl;
    return;

    file.write(data.c_str(), data.size());
    if (file.good())
        std::cout << "File written successfully: " << filename << std::endl;
    else
        std::cerr << "Error: Writing to file failed." << std::endl;

    file.close();
}

int main()
{
    std::string filename = "example.bin";
    std::string data = "���\n$�D6��ؓ���򵹺�q�e�h��ʸ����声�Y~��.�\"y�B�m}�����+#��\"ݖ�F����u�����\n";

    // Llama a la función para crear el archivo y escribir los datos
    writeToFile(filename, data, true);

    // Llama a la función para añadir más datos al archivo
    // writeToFile(filename, "\nMore binary data.", false);

    return 0;
}
