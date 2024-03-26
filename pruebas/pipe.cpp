//  los pipes (tuberías) son una forma de comunicación entre procesos.
//  Un pipe se utiliza para transferir datos desde la salida de un proceso a la entrada de otro proceso.
//  Los pipes son unidireccionales, es decir, los datos fluyen en una sola dirección.

// Para utilizar pipes en C++98, se emplean las funciones pipe, close, read y write, que se encuentran en la biblioteca <unistd.h>.

// pipe: Crea un par de descriptores de archivo que representan los extremos de un pipe. Se pasa un array de dos enteros como argumento, donde el primer elemento del array es el descriptor de archivo para la lectura y el segundo es el descriptor de archivo para la escritura.

// close: Cierra un descriptor de archivo. Esto es importante para asegurarse de que no haya fugas de recursos y para señalizar que no se van a leer ni escribir más datos en ese extremo del pipe.

// read: Lee datos desde un descriptor de archivo. Toma como argumentos el descriptor de archivo desde el cual leer, un buffer donde colocar los datos leídos y el tamaño del buffer.

// write: Escribe datos en un descriptor de archivo. Toma como argumentos el descriptor de archivo donde escribir, un buffer que contiene los datos a escribir y la cantidad de bytes a escribir.

#include <iostream>
#include <unistd.h>

int main()
{
	int pipefd[2];	 // Array para almacenar los descriptores de archivo del pipe
	char buffer[50]; // Buffer para almacenar los datos leídos del pipe
	pid_t pid;		 // Variable para almacenar el ID del proceso hijo

	// Crear el pipe
	if (pipe(pipefd) == -1)
	{
		perror("Error al crear el pipe");
		return 1;
	}

	// Crear un proceso hijo
	pid = fork();

	if (pid == -1)
	{
		perror("Error al crear el proceso hijo");
		return 1;
	}
	else if (pid == 0)
	{					  // Código del proceso hijo
		close(pipefd[1]); // Cerrar el extremo de escritura del pipe en el proceso hijo

		// Leer datos del pipe y mostrarlos por pantalla
		read(pipefd[0], buffer, sizeof(buffer));
		std::cout << "Proceso hijo: Mensaje recibido: " << buffer << std::endl;

		close(pipefd[0]); // Cerrar el extremo de lectura del pipe en el proceso hijo
	}
	else
	{					  // Código del proceso padre
		close(pipefd[0]); // Cerrar el extremo de lectura del pipe en el proceso padre

		// Escribir datos en el pipe
		const char *message = "Hola desde el proceso padre";
		write(pipefd[1], message, strlen(message) + 1);

		close(pipefd[1]); // Cerrar el extremo de escritura del pipe en el proceso padre
	}

	return 0;
}
