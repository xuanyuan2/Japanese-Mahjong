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
		exit(1);
	}
}

void Client::run() {
	loadTileTextures();

	// Test code from SFML
	// create the window
	sf::RenderWindow window(sf::VideoMode(m_width, m_height), "Japanese Mahjong");

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
		sf::Color mahjongGreen(70, 120, 100, 255);
		window.clear(mahjongGreen);

		// Draw stuff
		Client::draw(window);

		// end the current frame
		window.display();
	}
}

void Client::loadTileTextures() {
	// Load the textures of all tiles except the red dora and the facedown
	for (int i = 0; i < NUM_OF_TILE_TEXTURES - 3 ; i++) {
		sf::Uint8 tile = i * 4;
		if (!m_tileTextures[i].loadFromFile(filename(tile)))
			exit(1); // Images missing
	}

	// Load the red dora into the last three textures
	sf::Uint8 tile = PIN_5_RED; // Pin 5 red
	if (!m_tileTextures[34].loadFromFile(filename(tile)))
		exit(1);

	tile = SOU_5_RED; // Sou 5 red
	if (!m_tileTextures[35].loadFromFile(filename(tile)))
		exit(1);

	tile = WAN_5_RED; // Wan 5 red
	if (!m_tileTextures[36].loadFromFile(filename(tile)))
		exit(1);

	// Load the facedown tile texture
	if (!m_facedownTileTexture.loadFromFile("tiles/facedown.png"))
		exit(1);
}

int Client::tileTextureNo(sf::Uint8 tile) {
	int index;
	if (isRed(tile)) {
		if (tile = PIN_5_RED) return 34;
		if (tile = SOU_5_RED) return 35;
		if (tile = WAN_5_RED) return 36;
	}
	else index = typeOf(tile);
	return index;
}

void Client::draw(sf::RenderWindow& window) {
	// Testing 
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

	sf::Uint8 testTile = PIN_5_RED; 
	sf::Sprite testSprite;
	testSprite.setTexture(m_tileTextures[tileTextureNo(testTile)]);
	testSprite.setPosition(sf::Vector2f(0,0));
	window.draw(testSprite);
}

