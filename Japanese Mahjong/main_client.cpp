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

#include "../MPacket.h"
#include "../Tile.h"

// Player specifies his/her username
sf::String chooseUsername() {
	while (true) {
		std::cout << "Please choose a username: ";
		
		std::string input;
		std::getline(std::cin, input);

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

int main()
{
	std::cout << "Welcome to Japanese Mahjong!" << std::endl;

	sf::String username = chooseUsername();

	// Connection attempt loop

	sf::TcpSocket socket;
	bool loop = true;
	while (loop) {
		sf::IpAddress ip = chooseIP();
		int port = choosePort();

		if (socket.connect(ip, port) != sf::Socket::Done) { // Attempt to connect
			// connect() failed
			std::cout << "Connection failed." << std::endl;
		}
		else {
			// connect() succeeded
			std::cout << "Connection succeeded." << std::endl;

			InitPacket initPacket = InitPacket(username);			
			sf::Packet packet;
			packet << initPacket;

			if (socket.send(packet) != sf::Socket::Done) {
				// Packet delivery failed
				std::cout << "Packet transmission error!" << std::endl;
				return 1;
			}

			loop = false;
		}
	}

	while(true) {
	}
	// Client class initialization goes here

    return 0;
}
