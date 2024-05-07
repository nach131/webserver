#include <iostream>
// #include <types.h>
#include <sys/event.h>
// #include <arpa/inet.h>
// #include <sys/types.h>
// #include <sys/event.h>

// Estructura para almacenar información del cliente
struct ClientInfo {
    int clientId;
    std::string username;
    // Otros campos relevantes
};

// Función para imprimir la información del cliente
void printClientInfo(const ClientInfo* client) {
    std::cout << "Cliente ID: " << client->clientId << std::endl;
    std::cout << "Nombre de usuario: " << client->username << std::endl;
    // Imprimir otros campos relevantes
}

int main() {
    // Crear una estructura de información de cliente
    ClientInfo* client = new ClientInfo;
    client->clientId = 1234;
    client->username = "usuario1";

    // Crear un evento y asociar la información del cliente con él
    struct kevent event;
    EV_SET(&event, 123, EVFILT_READ, EV_ADD, 0, 0, client);

    ClientInfo* clientFromEvent = static_cast<ClientInfo*>(event.udata);
    // Imprimimos la información del cliente asociada con el evento
    std::cout << "Información del cliente asociada con el evento:" << std::endl;
    printClientInfo(clientFromEvent);

    // Liberar la memoria asignada a la estructura de información del cliente
    delete client;

    return 0;
}
