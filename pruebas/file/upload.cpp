#include <iostream>
#include <string>
#include <cstdlib>

int main()
{
    std::string py = "./upload.py";
    std::string filename = "output.txt";
    std::string data = "This is the data to be written to the file";

    // Escape quotes and special characters in data
    std::string escaped_data = "";
    for (size_t i = 0; i < data.size(); ++i)
    {
        if (data[i] == '\"' || data[i] == '\\')
            escaped_data += '\\';
        escaped_data += data[i];
    }

    // Form the command string
    std::string command = "python3 " + py + " \"" + filename + "\" \"" + escaped_data + "\" > ./tmp.txt";

    // Execute the command
    int result = std::system(command.c_str());

    // Check the result
    if (result == 0)
    {
        std::cout << "Script executed successfully." << std::endl;
    }
    else
    {
        std::cerr << "Error: Script execution failed with code " << result << std::endl;
    }

    return 0;
}
