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

This is the main file of the SERVER for Japanese Mahjong.
*/
#include <iostream>
#include <sstream>
#include <SFML/Network.hpp>
#include <SFML/Network/IpAddress.hpp>

const int NUMPLAYERS = 2; // Should actually be 4, of course

sf::String usernames[NUMPLAYERS];

// Host specifies server port. Returns the valid port chosen by the host.
int choosePort() {
	while(true) {
		int port; 

		std::cout << "Please enter port number (or hit enter for default): ";

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

// Handles accepting an incoming client from listener into the clients[] array
// Stores usernames (includes dealing with duplicates)
// Reports player connection
// Informs player of prior connections
void acceptClient(sf::TcpListener& listener, sf::TcpSocket& client, int index) {
	listener.accept(client);

	// Receive information from player
	sf::Packet packet;
	if (client.receive(packet) != sf::Socket::Done) {
		std::cout << "Packet transmission error!" << std::endl;
		exit(1);
	}
	sf::String username;
	packet >> username;
	
	// Ensure no duplicate usernames
	int duplicates = 1;
	for (int i = 0; i < index; i++) { // For previously connected players
		if (usernames[i] == username) {
			// Identical username detected!
			if (duplicates == 1) { // First duplicate
				username += std::to_string(duplicates); // Adds a "1" to the end of the name
				duplicates++;
			}
			else { // Other duplicates already exist
				duplicates++;
				username = username.substring(0, username.getSize() - 1) + std::to_string(duplicates);
			}
		}
	}
	sf::String newUsername; // Defaults to empty string, which client interprets as no change
	if (duplicates >= 2) {
		// Duplicates were found - send edited username
		newUsername = (username); 
	}
	packet.clear();
	packet << newUsername;
	if (client.send(packet) != sf::Socket::Done) {
		std::cout << "Packet transmission error!" << std::endl;
		exit(1);
	}

	// Send player information on players that have already connected
	packet.clear();
	packet << (sf::Int8)index; // Number of player info packets to be sent; also player #
	for (int i = 0; i < index; i++)
		packet << usernames[i];
	if (client.send(packet) != sf::Socket::Done) {
		std::cout << "Packet transmission error!" << std::endl;
		exit(1);
	}

	usernames[index] = username; // Store the correct username away
	std::cout << "Player " << index + 1 << ", named \"" << username.toAnsiString() << "\", has connected from " << client.getRemoteAddress() << "!" << std::endl;
}

int main()
{
	std::cout << "Welcome to the Japanese Mahjong server!" << std::endl;

	int port = choosePort();
	
	// Bind TCP listener to port
	sf::TcpListener listener;
	if (listener.listen(port) != sf::Socket::Done) {
		std::cout << "Failed to bind to port! Please restart server." << std::endl;
		return 1;
	}

	std::cout << "Your external IP appears to be: " << sf::IpAddress::getPublicAddress() << std::endl;

	std::cout << "Attempting to find players (hit Ctrl+C to abort)..." << std::endl;

	sf::TcpSocket clients[NUMPLAYERS];
	for (int i = 0; i < NUMPLAYERS; i++) {
		acceptClient(listener, clients[i], i);

		// Inform clients about the newest client to join
		sf::Packet packet;
		packet << usernames[i];
		for (int j = 0; j < i; j++) {
			if (clients[j].send(packet) != sf::Socket::Done) {
				std::cout << "Packet transmission failed!" << std::endl;
				return 1;
			}
		}
	}

	std::cout << "All players have connected!" << std::endl;

	std::cout << "Press ENTER to begin.";
	std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');

	while(true) {}
	// Server class initialization here

	std::cout << "Game over, server shutting down." << std::endl;
    return 0;
}
