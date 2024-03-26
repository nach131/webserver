#include <iostream>
#include <vector>

// Enumeración para tipos de eventos
enum class EventType
{
	EVENT_A,
	EVENT_B
};

// Estructura para representar un evento
struct Event
{
	EventType type;
	int data; // Datos asociados con el evento (puede variar dependiendo del tipo de evento)
};

// Clase que maneja eventos
class EventHandler
{
public:
	// Método para suscribirse a un tipo de evento
	void subscribe(EventType eventType, void (*callback)(const Event &))
	{
		callbacks[eventType].push_back(callback);
	}

	// Método para desuscribirse de un tipo de evento
	void unsubscribe(EventType eventType, void (*callback)(const Event &))
	{
		std::vector<void (*)(const Event &)> &subscribers = callbacks[eventType];
		for (auto it = subscribers.begin(); it != subscribers.end(); ++it)
		{
			if (*it == callback)
			{
				subscribers.erase(it);
				break;
			}
		}
	}

	// Método para notificar un evento a los suscriptores
	void notify(const Event &event)
	{
		for (const auto &callback : callbacks[event.type])
		{
			callback(event);
		}
	}

private:
	// Almacena los callbacks asociados con cada tipo de evento
	std::vector<void (*)(const Event &)> callbacks[static_cast<int>(EventType::EVENT_B) + 1];
};

// Ejemplo de función de callback
void onEvent(const Event &event)
{
	std::cout << "Evento de tipo ";
	switch (event.type)
	{
	case EventType::EVENT_A:
		std::cout << "EVENT_A ";
		break;
	case EventType::EVENT_B:
		std::cout << "EVENT_B ";
		break;
	}
	std::cout << "recibido con datos: " << event.data << std::endl;
}

int main()
{
	EventHandler eventHandler;

	// Suscribirse a EVENT_A
	eventHandler.subscribe(EventType::EVENT_A, onEvent);

	// Notificar evento EVENT_A
	Event eventA = {EventType::EVENT_A, 42};
	eventHandler.notify(eventA);

	// Desuscribirse de EVENT_A
	eventHandler.unsubscribe(EventType::EVENT_A, onEvent);

	// Notificar evento EVENT_A nuevamente (nadie debería recibir este evento)
	eventHandler.notify(eventA);

	return 0;
}
