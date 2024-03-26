// htonl es una función que convierte un número entero de 32 bits del formato de host al formato de red.
//  Al igual que htons, htonl es útil cuando se trabaja con protocolos de red, ya que asegura que
//   los datos se interpreten correctamente en sistemas con diferentes arquitecturas de procesador que
//    puedan tener diferentes representaciones de bytes.

// uint32_t htonl(uint32_t hostlong);

// hostlong: El número entero de 32 bits en formato de host que se desea convertir al formato de red.

#include <iostream>
#include <arpa/inet.h>

int main()
{
	uint32_t host_int = 16909060;			// Número entero de 32 bits en formato de host (representa la dirección IP 1.2.3.4)
	uint32_t network_int = htonl(host_int); // Convertir el número entero al formato de red

	std::cout << "Número entero en formato de host: " << host_int << std::endl;
	std::cout << "Número entero en formato de red: " << network_int << std::endl;

	return 0;
}

// Número entero en formato de host: 16909060
// Número entero en formato de red: 67305985