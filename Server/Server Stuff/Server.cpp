#include "Server.h"
#include <iostream>
#include <future>

Server::Server(uint16_t portToListen) {
	// bind the listener to a port	
	if (listener.listen(portToListen) != sf::Socket::Done) {
		std::cerr << "Could not listen to port: " << listener.getLocalPort() << " for whatever reason :/";
		system("pause");
		exit(1);
	}

	std::cout << "SERVER IS RUNNING YOO!! WE LISTENING ON PORT " << listener.getLocalPort() << "\n";
}

void Server::runServer(){
	listenForClients();

	auto future = std::async(std::launch::async, &Server::sendMessage, this);
	receiveMessage();
}

void Server::sendMessage(){
	std::string message, server = "Server: ";
	sf::Packet sendPacket;
	sf::Socket::Status status;

	do {
		//	std::cout << "Enter message >";
		std::getline(std::cin, message);

		//insert the message we will be sending to the client into our packet
		sendPacket << message;
		status = client.send(sendPacket);
		sendPacket.clear();
	} while (status != sf::Socket::Disconnected);

	std::cout << "Disconnected!\n"; 
}

void Server::receiveMessage(){
	std::string message;
	sf::Packet receivePacket;
	sf::Socket::Status status;

	do {
		//In addition sending a message in a Packet, we will also be receiving a message from the client!
		status = client.receive(receivePacket);

		//Error handle to make sure that we aren't extracting more data from the packet than what was originally there
		if (receivePacket >> message) {
			std::cout << "Client: " + message << "\n";
		}

	} while (status != sf::Socket::Disconnected);

	std::cout << "Disconnected!\n";
}

void Server::listenForClients(){
	std::cout << "waiting for a connection...\n";
	if (listener.accept(client) == sf::Socket::Done) {

		// A new client just connected!
		std::cout << "New connection received from " << client.getRemoteAddress() << std::endl;
	}
}