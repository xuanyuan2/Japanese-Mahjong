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

#ifndef CLIENT_H
#define CLIENT_H
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "../Tile.h"
#include "../MPacket.h"

const std::string FONT = "sazanami-gothic.ttf";

class Client {
public:
	Client(sf::TcpSocket& server, int NUMPLAYERS, sf::String usernames[]); // Constructor
	void run(); // Order the client to run the game
private:
	// Renders the 2D state of the world to the window in preparation for display
	void draw(sf::RenderWindow& window);

	sf::TcpSocket& m_server;
	int m_NUMPLAYERS;
	sf::String* m_usernames;
	sf::Font m_font;
};
#endif