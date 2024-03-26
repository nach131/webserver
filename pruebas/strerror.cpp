// La función strerror se utiliza para obtener una cadena de caracteres que describe el último error
// que ocurrió en el sistema. Este error se identifica mediante un número entero, generalmente
// proporcionado por alguna función de sistema que devuelve un valor negativo para indicar un error.

// En C++98, strerror se encuentra en la biblioteca <cstring>, aunque también puede estar disponible en <string.h>.

// char *strerror(int errnum);

// errnum: El número de error para el cual se quiere obtener una descripción.

#include <iostream>
#include <cstring> // Para strerror
#include <cerrno>  // Para errno

int main()
{
	FILE *file = fopen("archivo_no_existente.txt", "r");

	if (file == NULL)
	{
		std::cerr << "Error al abrir el archivo: " << strerror(errno) << std::endl;
	}
	else
	{
		// Realizar operaciones con el archivo
		fclose(file);
	}

	return 0;
}
