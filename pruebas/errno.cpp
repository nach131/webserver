// En C++98, errno es una variable global que se utiliza para almacenar el número de error del último
//  error que ocurrió en el sistema. Los errores se indican mediante números enteros negativos y
//   están definidos en la biblioteca de cabecera <cerrno>.

// errno es muy útil para diagnosticar errores en funciones que no devuelven un código de error explícito,
//  sino que indican un error a través de un valor de retorno especial, como NULL o - 1.

#include <iostream>
#include <cstdio>
#include <cerrno> // Para errno

int main()
{
	FILE *file = fopen("archivo_no_existente.txt", "r");

	if (file == NULL)
	{
		// Algo salió mal al abrir el archivo
		if (errno == ENOENT)
			std::cerr << "El archivo no existe." << std::endl;
		else if (errno == EACCES)
			std::cerr << "No se puede acceder al archivo debido a permisos insuficientes." << std::endl;
		else
			std::cerr << "Error al abrir el archivo: " << strerror(errno) << std::endl;
	}
	else
	{
		// Operaciones con el archivo
		fclose(file);
	}

	return 0;
}
