#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <future>
#include <algorithm>
#include "Random.h"

void sendMessage(sf::TcpSocket &serverSocket) {
	std::string message, client = "Client: ";
	sf::Packet sendPacket;
	sf::Socket::Status status;
	
	do {
		//std::cout << "Enter message >";
		std::getline(std::cin, message);
		//std::cout << message << "\n";
		//insert the message we will be sending to the client into our packet
		sendPacket << message;
		status = serverSocket.send(sendPacket);
		sendPacket.clear();
	} while (status != sf::Socket::Disconnected);

	std::cout << "Disconnected!\n";
}

void receiveMessage(sf::TcpSocket &serverSocket) {
	std::string message;
	sf::Packet receivePacket;
	sf::Socket::Status status;

	do{
		//std::cout << "\nReceiving message...\n";
		//In addition sending a message in a Packet, we will also be receiving a message from the Server!
		status = serverSocket.receive(receivePacket);

		//Error handle to make sure that we aren't extracting more data from the packet than what was originally there
		if (receivePacket >> message) {
			std::cout << "Server: " << message << "\n";
		}

	} while (status != sf::Socket::Disconnected);

	std::cout << "Disconnected!\n";
 }

int main() {
	sf::TcpSocket serverSocket;

	const std::string PRIVATE_IP = "192.168.0.32";
	const std::string PUBLIC_IP = "67.85.105.60";
	auto status = serverSocket.connect(sf::IpAddress::LocalHost, 2000);

	if (status != sf::Socket::Done) {
		std::cout << "Could not connect to server :(\n";
		system("pause");
		exit(1);
	}

	switch (status){
		case sf::Socket::Disconnected:
			std::cout << "Disconnected\n";
			break;

		case sf::Socket::Done:
			std::cout << "Done\n";
			std::cout << "Client just connected! Woo! :)\n";
			break;

		case sf::Socket::Error:
			std::cout << "Error\n";
			break;

		case sf::Socket::NotReady:
			std::cout << "Not ready >.>\n";
			break;

		case sf::Socket::Partial:
			std::cout << "Partial\n";
			break;
		default:
			break;
	}

	auto future = std::async(std::launch::async, sendMessage, std::ref(serverSocket));
	receiveMessage(serverSocket);
}