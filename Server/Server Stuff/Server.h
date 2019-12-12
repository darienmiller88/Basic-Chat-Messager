#pragma once
#include <SFML/Network.hpp>
#include <unordered_map>

/*Bare Bones C++ server using SFML networking utilities*/
class Server{
	public:
		Server(uint16_t portToListen);
		void runServer();
	private:
		void sendMessage();
		void receiveMessage();
		void listenForClients();
	private:
		sf::TcpListener listener;
		sf::TcpSocket client;//Socket of the client computer.
		sf::Socket::Status status;
		std::vector< std::unique_ptr<sf::TcpSocket> > clients;
};