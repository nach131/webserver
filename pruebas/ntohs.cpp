// En redes, los números de puerto también siguen la convención de orden de bytes de red, donde el byte
//  más significativo (MSB) se almacena primero. Sin embargo, cuando se utilizan en el contexto del
//  sistema operativo o del programa, generalmente se requiere que estén en el formato de host local.

// En C++98, ntohs se encuentra en la biblioteca <arpa/inet.h>.

// uint16_t ntohs(uint16_t netshort);

//     netshort: El número de puerto en el formato de red que se desea convertir al formato de host.

// La función ntohs devuelve el número de puerto en el formato de host.

#include <iostream>
#include <arpa/inet.h>

int main()
{
	uint16_t network_port = 8080;			  // Puerto en formato de red
	uint16_t host_port = ntohs(network_port); // Convertir el puerto al formato de host

	std::cout << "Puerto en formato de red: " << network_port << std::endl;
	std::cout << "Puerto en formato de host: " << host_port << std::endl;

	return 0;
}

// Puerto en formato de red: 8080
// Puerto en formato de host: 36895