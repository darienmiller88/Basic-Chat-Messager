#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <future>
#include "Random.h"
#include "Server.h"

void sendMessage(sf::TcpSocket &client) {
	std::string message, server = "Server: ";
	sf::Packet sendPacket;
	sf::Socket::Status status;

	do{
	//	std::cout << "Enter message >";
		std::getline(std::cin, message);
		std::cout << "\rServer: " + message << "\n";
		//insert the message we will be sending to the client into our packet
		sendPacket << message;
		status = client.send(sendPacket);
		sendPacket.clear();
	} while (status != sf::Socket::Disconnected);
	
	std::cout << "Disconnected!\n";
}

void receiveMessage(sf::TcpSocket &client) {
	std::string message;
	sf::Packet receivePacket;
	sf::Socket::Status status;

	do {
		//In addition sending a message in a Packet, we will also be receiving a message from the client!
		status = client.receive(receivePacket);

		//Error handle to make sure that we aren't extracting more data from the packet than what was originally there
		if (receivePacket >> message) {
			std::cout << message << "\n";
		}

	} while (status != sf::Socket::Disconnected);

	std::cout << "Disconnected!\n";
}

void listenForClients(sf::TcpListener &listener, sf::TcpSocket &client) {
	//std::cout << "waiting for a connection...\n";
	
	if (listener.accept(client) == sf::Socket::Done) {
		
		// A new client just connected!
		std::cout << "New connection received from " << client.getRemoteAddress() << std::endl;
	}
}

void bindToPort(sf::TcpListener &listener, uint16_t port) {
	// bind the listener to a port	
	if (listener.listen(port) != sf::Socket::Done) {
		std::cerr << "Could not listen to port: " << port << " for whatever reason :/";
		system("pause");
		exit(1);
	}

	std::cout << "SERVER IS RUNNING YOO!! WE LISTENING ON PORT " << listener.getLocalPort() << "\n";
}

int main() {
	//RenderWindow window({ 600, 600 }, "circles", Style::Close | Style::Titlebar);
	constexpr uint16_t PORT = 2000;
	Server server(PORT);

	server.runServer();
	
	/*while (window.isOpen()) {
		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed)
				window.close(); 
		}

		
		window.clear();
		window.display();
	}*/
	system("pause");
}