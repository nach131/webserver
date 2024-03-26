// htons es una función que convierte un número de puerto de un formato de host a un formato de red.
// En la mayoría de los sistemas, el formato de red utiliza el byte más significativo (MSB) primero,
//  mientras que el formato de host utiliza el byte menos significativo (LSB) primero.
//  Esto es importante cuando se trabaja con protocolos de red, ya que el orden de los bytes en un
//  número de puerto puede afectar la forma en que se interpretan los datos.

// uint16_t htons(uint16_t hostshort);

//   hostshort: El número de puerto en el formato de host que se desea convertir al formato de red.

// La función htons devuelve el número de puerto en el formato de red.

#include <iostream>
#include <arpa/inet.h>

int main()
{
	uint16_t host_port = 8080;				  // Puerto en formato de host
	uint16_t network_port = htons(host_port); // Convertir el puerto al formato de red

	std::cout << "Puerto en formato de host: " << host_port << std::endl;
	std::cout << "Puerto en formato de red: " << network_port << std::endl;

	return 0;
}

// Puerto en formato de host: 8080
// Puerto en formato de red: 36895