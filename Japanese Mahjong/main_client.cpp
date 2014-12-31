/*
Designed by Xuan Yuan Huang, a desperate UCLA undergrad CS student.
Copyright 2014 Xuan Yuan Huang

Built with the SFML 2.2 Engine.
SFML 2.2 is licensed under zlib/libpng. 

For my code:
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
Because I'm a desperate undergrad that needs to prove his worth.

Developed in Visual Studio 2013 (thanks for the free copy, MS Dreamspark!).
Visual Studio is, of course, Microsoft's proprietary IDE.

This is the main file of the CLIENT of the Japanese Mahjong program.
*/
#include <iostream>
#include <sstream>
#include <SFML/Network.hpp>
#include <SFML/Network/Socket.hpp>

#include "Client.h"

const int NUMPLAYERS = 4;

// Player specifies his/her username
sf::String chooseUsername() {
	while (true) {
		std::cout << "Please enter a username: ";
		
		std::string input;
		std::getline(std::cin, input);

		if (input.empty()) {
			std::cout << "Empty usernames are not acceptable." << std::endl;
			continue;
		}
		else if (input.find_first_not_of(' ') == std::string::npos) {
			// There are no non-whitespace characters in the string
			std::cout << "Username should not consist solely of whitespaces." << std::endl;
			continue;
		}

		sf::String result(input);
		return result;
	}
}

// Player specifies server IP. Returns the IP chosen by the player (DOES NOT CHECK FOR VALIDITY).
sf::IpAddress chooseIP() {
	while(true) {
		std::cout << "Please enter server IP address: ";

		std::string input;
		std::getline(std::cin, input);

		sf::IpAddress result(input);
		return result;
	}
}

// Player specifies server port. Returns the valid port chosen by the player.
int choosePort() {
	while(true) {
		int port; 

		std::cout << "Please enter server port number (or hit enter for default): ";

		std::string input;
		std::getline(std::cin, input);

		std::stringstream sstream = std::stringstream(input);

		if (sstream.rdbuf()->in_avail() == 0) { // If user input was empty
			std::cout << "Defaulting to port 53000." << std::endl;
			return 53000;
		}
		else {			
			sstream >> port;

			if (port < 1024 // These ports are system reserved
				|| port > 65535) { // Maximum TCP port number
				std::cout << "Invalid port number!" << std::endl;
			}
			else return port;
		}
	}
}

// Handles connecting to a server on socket. Returns the player's # on the server
int connectToServer(sf::TcpSocket& socket, sf::String& username, sf::String usernames[]) {
	// Loop until connection succeeds
	while (true) {
		sf::IpAddress ip = chooseIP();
		int port = choosePort();

		// Attempt to connect
		if (socket.connect(ip, port) != sf::Socket::Done) { 
			// connect() failed
			std::cout << "Connection failed." << std::endl;
		}
		else {
			// connect() succeeded
			std::cout << "Connection succeeded." << std::endl;

			// Send username
			sf::Packet packet;
			packet << username;
			if (socket.send(packet) != sf::Socket::Done) {
				std::cout << "Packet transmission error!" << std::endl;
				exit(1);
			}

			// Check if a new username has arrived
			packet.clear();
			if (socket.receive(packet) != sf::Socket::Done) {
				std::cout << "Packet transmission error!" << std::endl;
				exit(1);
			}
			sf::String newUsername;
			packet >> newUsername;
			if (!newUsername.isEmpty()) { // Server enforced name change
				username = newUsername; // Save new username
				std::cout << "Player(s) already exist on the server with your username!" << std::endl
					<< "Your username has been changed to \"" << username.toAnsiString() << "\"." << std::endl;
			}

			// Parse information on other players in the server
			packet.clear();
			if (socket.receive(packet) != sf::Socket::Done) {
				std::cout << "Packet transmission error!" << std::endl;
				exit(1);
			}
			sf::Int8 playerNo;
			packet >> playerNo;
			std::cout << "You are Player " << (int)playerNo + 1 << "!" << std::endl;
			usernames[playerNo] = username;

			for (int i = 0; i < playerNo; i++) {
				sf::String otherPlayerUsername;
				packet >> otherPlayerUsername;
				std::cout << "Player " << i + 1 << " is \"" << 
					otherPlayerUsername.toAnsiString() << "\"." << std::endl;
				usernames[i] = otherPlayerUsername;
			}

			return playerNo;
		}
	}
}

// Listen for more players from the server
void waitOnMorePlayers(sf::TcpSocket& socket, sf::Int8 playerNo, sf::String usernames[]) {
	for (int i = playerNo + 1; i < NUMPLAYERS; i++) {
		sf::Packet packet;
		if (socket.receive(packet) != sf::Socket::Done) {
			std::cout << "Packet receipt failed!" << std::endl;
			exit(1);
		}
		else {
			sf::String newPlayerUsername;
			packet >> newPlayerUsername;
			std::cout << "Player " << i + 1 << " is \"" << 
				newPlayerUsername.toAnsiString() << "\"." << std::endl;
			usernames[i] = newPlayerUsername;
		}
	}
	std::cout << "All players have connected!" << std::endl;
}

int main() {
#ifndef CLIENT_ONLY_DEBUG // Define if you only want to test client alone
	std::cout << "Welcome to Japanese Mahjong!" << std::endl;

	sf::String username = chooseUsername();

	sf::TcpSocket socket;
	sf::String usernames[NUMPLAYERS];
	sf::Int8 playerNo = connectToServer(socket, username, usernames);
	waitOnMorePlayers(socket, playerNo, usernames);

	std::cout << "Waiting for server host to start game..." << std::endl;
	// Wait for (empty) packet declaring start of game
	sf::Packet packet;
	if (socket.receive(packet) != sf::Socket::Done) {
		std::cout << "Packet transmission failed!" << std::endl;
		return 1;
	}

	std::cout << "Launching game!" << std::endl;
	Client client(socket, NUMPLAYERS, usernames);
	client.run();
#else
	sf::TcpSocket fakeSocket;
	sf::String usernames[NUMPLAYERS];
	usernames[0] = "foo";
	usernames[1] = L"漣";
	Client client(fakeSocket, NUMPLAYERS, usernames);
	client.run();
#endif

    return 0;
}
