// dup2 es una función de Unix que se utiliza para duplicar un descriptor de archivo.
//  Esto significa que puedes redirigir un descriptor de archivo a otro descriptor de archivo existente.
// La función dup2 toma dos argumentos: el descriptor de archivo que se quiere duplicar y el descriptor de archivo al que se quiere duplicar.
//  Si el descriptor de archivo al que se quiere duplicar ya está abierto, primero se cierra.

// int dup2(int oldfd, int newfd);

#include <iostream>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	// Abrir o crear un archivo para escribir
	int fileDescriptor = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

	if (fileDescriptor == -1)
	{
		std::cerr << "Error al abrir o crear el archivo." << std::endl;
		return 1;
	}

	// Duplicar el descriptor de archivo del archivo abierto a stdout (descriptor de archivo 1)
	if (dup2(fileDescriptor, STDOUT_FILENO) == -1)
	{
		std::cerr << "Error al duplicar el descriptor de archivo." << std::endl;
		return 1;
	}

	// Cerrar el descriptor de archivo duplicado
	close(fileDescriptor);

	// Ahora la salida estándar se redirigirá al archivo "output.txt"
	std::cout << "Este texto se escribirá en el archivo output.txt" << std::endl;

	return 0;
}

// En este ejemplo, el programa abre o crea un archivo llamado "output.txt".
// Luego, usa dup2 para duplicar el descriptor de archivo del archivo abierto al descriptor de archivo de salida estándar (STDOUT_FILENO,
// que tiene el valor 1). Después de duplicar el descriptor de archivo, el programa escribe en la salida estándar como de costumbre,
//  pero los datos se redirigen al archivo "output.txt". Finalmente, cierra el descriptor de archivo del archivo "output.txt".