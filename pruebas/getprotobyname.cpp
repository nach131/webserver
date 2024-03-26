//  se utiliza para obtener información sobre un protocolo de red utilizando su nombre.
//  Esta función es útil cuando necesitas obtener el número de protocolo asociado con
//  un nombre de protocolo conocido, como "tcp", "udp", "icmp", etc.

// struct protoent *getprotobyname(const char *name);

//     name: El nombre del protocolo que se desea buscar.

// La función devuelve un puntero a una estructura protoent que
// contiene información sobre el protocolo buscado. Si no se encuentra
// el protocolo o si ocurre un error, la función devuelve un puntero nulo.

struct protoent
{
	char *p_name;	  // Nombre del protocolo
	char **p_aliases; // Alias del protocolo
	int p_proto;	  // Número del protocolo
};

#include <iostream>
#include <netdb.h>

int main()
{
	const char *protocol_name = "tcp";
	struct protoent *protocol_info;

	// Obtener información sobre el protocolo TCP
	protocol_info = getprotobyname(protocol_name);
	if (protocol_info == NULL)
	{
		std::cerr << "Error al obtener información sobre el protocolo: " << hstrerror(h_errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	// Imprimir información sobre el protocolo TCP
	std::cout << "Nombre del protocolo: " << protocol_info->p_name << std::endl;
	std::cout << "Número del protocolo: " << protocol_info->p_proto << std::endl;

	return 0;
}

// Nombre del protocolo: tcp
// Número del protocolo: 6