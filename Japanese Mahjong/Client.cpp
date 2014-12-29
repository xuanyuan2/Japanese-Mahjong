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

This file handles the operation of the client.
*/

#include <iostream>

#include "Client.h"

Client::Client(sf::TcpSocket& server, int NUMPLAYERS, sf::String usernames[]) :
	m_server(server)
{ // Constructor for client class
	m_NUMPLAYERS = NUMPLAYERS;
	m_usernames = usernames;

	if (!m_font.loadFromFile(FONT)) {
		std::cout << "Font load error! Aborting..." << std::endl;
		exit(1);
	}
}

void Client::run() {
	// Test code from SFML
	// create the window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Japanese Mahjong");

	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// clear the window
		window.clear(sf::Color::Green);

		// Draw stuff
		Client::draw(window);

		// end the current frame
		window.display();
	}
}

void Client::draw(sf::RenderWindow& window) {
	for (int i = 0; i < m_NUMPLAYERS; i++) {
		sf::Text username;
		username.setFont(m_font);
		sf::String text = "Player " + std::to_string(i + 1) + ": " + m_usernames[i];
		username.setString(text);
		username.setCharacterSize(18);
		username.setColor(sf::Color::Magenta);
		username.move(0, (float)30 * i);
		window.draw(username);
	}
}

