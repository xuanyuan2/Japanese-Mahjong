/*
Designed by Xuan Yuan Huang, a desperate UCLA undergrad CS student.
Copyright 2014 Xuan Yuan Huang

Built with the SFML 2.1 Engine.
SFML 2.1 is licensed under zlib/libpng. 

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

Developed in Visual Studio 2012 (thanks for the free copy, MS Dreamspark!).
Visual Studio is, of course, Microsoft's proprietary IDE.

This is the main file of the CLIENT of the Japanese Mahjong program.
*/
#include <iostream>
#include <sstream>
#include <SFML/Network.hpp>
#include <SFML/Network/Socket.hpp>

// Player specifies server IP. Returns the IP chosen by the player (DOES NOT CHECK FOR VALIDITY).
sf::IpAddress chooseIP() {
	while(true) {
		std::string address;

		std::cout << "Please enter server IP address: ";

		std::string input;
		std::getline(std::cin, input);

		std::stringstream sstream = std::stringstream(input);
			
		sstream >> address;

		sf::IpAddress result(address);

		return result;
	}
}

// Player specifies server port. Returns the valid port chosen by the player.
int choosePort() {
	while(true) {
		int port; 

		std::cout << "Please enter server port number: ";

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
	// Connection attempt loop
	while (true) {
		sf::IpAddress ip = chooseIP();
		int port = choosePort();

		sf::TcpSocket socket;

		if (socket.connect(ip, port) == sf::Socket::Done) { // Attempt to connect
			// connect() failed
			std::cout << "Connection failed." << std::endl;
		}
		else break; // Connection established
	}

	while(true) {
		// Game loop
	}
    return 0;
}
