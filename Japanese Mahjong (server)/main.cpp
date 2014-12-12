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
*/
#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Network/SocketSelector.hpp>

const int NUMPLAYERS = 1; // Should actually be 4, of course
const bool DEBUG = true; // Set to false when compiling binaries!

int main()
{

	sf::TcpListener listener;

	// Bind to port

	if (listener.listen(53000) != sf::Socket::Done) {
		std::cout << "Failed to bind to port! Please restart server." << std::endl;
		return 1;
	}

	std::cout << "Attempting to find players..." << std::endl;

	sf::TcpSocket clients[NUMPLAYERS];
	for (int i = 0; i < NUMPLAYERS; i++) {
		listener.accept(clients[i]);
		std::cout << "Player " << i << " has connected!" << std::endl;
	}

	std::cout << "All players have connected!" << std::endl;


	while(true){
		// game loop
	}

	std::cout << "Game over, server shutting down." << std::endl;
	// Future - possibly let host hit y to let server start again?
    return 0;
}
