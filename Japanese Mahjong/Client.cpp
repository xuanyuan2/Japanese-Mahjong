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
#include <SFML/Audio.hpp>

#include "Client.h"
#include "../MPacket.h"

// Constructor for client class
Client::Client(sf::TcpSocket& server, int NUMPLAYERS, sf::String usernames[], sf::Int8 playerNo) :
	m_server(server)
{ 
	m_NUMPLAYERS = NUMPLAYERS;
	m_usernames = usernames;
	m_playerNo = playerNo;

	if (!m_font.loadFromFile(FONT)) {
		exit(1);
	}
}

void Client::run() {
	loadTileTextures();

	// This code is for debugging
	sf::Packet packet;
	m_server.receive(packet);
	FirstHandPacket firstHand;
	packet >> firstHand;
	const Tile* firstTiles = firstHand.getHand();
	for (int i = 0; i < 13; i++) {
		hand[i] = firstTiles[i];
	}
	std::sort(std::begin(hand), std::end(hand));
	DrawPacket firstDraw;
	packet.clear();
	m_server.receive(packet);
	packet >> firstDraw;
	drawnTile = firstDraw.getDraw();

	sf::Music bgm;
	if (!bgm.openFromFile("music.ogg")) exit(1);
	bgm.play();
	
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
			// TODO: Send shutdown signal to server and other clients
			// The follwoing elseif clause is VERY much debug code! Magic numbers, ahoy!
			else if (event.type == sf::Event::MouseButtonReleased) {
				sf::Vector2i pos = sf::Mouse::getPosition(window);
				std::cout << "(" << pos.x << ", " << pos.y << ")" << std::endl;
				// Hand
				if (pos.x > 100 && pos.x < 646 &&
					pos.y > 526 && pos.y < 600) {
					int i = floor((pos.x - 100) / 42);
					discard(i);
				}
				// Draw
				else if (pos.x > 698 && pos.x < 740 &&
					pos.y > 526 && pos.y < 600){
					discard(14);
				}
			}
		}

		// clear the window
		sf::Color mahjongGreen(70, 120, 100, 255);
		window.clear(mahjongGreen);

		// Draw stuff
		Client::render(window);

		// end the current frame
		window.display();
	}

	bgm.stop();
}

void Client::loadTileTextures() {
	// Load the textures of all tiles except the red dora and the facedown
	for (int i = 0; i < NUM_OF_TILE_TEXTURES - 3 ; i++) {
		Tile tile = i * 4;
		if (!m_tileTextures[i].loadFromFile(filename(tile)))
			exit(1); // Images missing
	}

	// Load the red dora into the last three textures
	Tile tile = PIN_5_RED; // Pin 5 red
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

int Client::tileTextureNo(Tile tile) {
	int index;
	if (isRed(tile)) {
		if (tile = PIN_5_RED) return 34;
		if (tile = SOU_5_RED) return 35;
		if (tile = WAN_5_RED) return 36;
	}
	else index = typeOf(tile);
	return index;
}

void Client::render(sf::RenderWindow& window) {
	// Render the hand (still testing)
	for (int i = 0; i < 13; ++i) {
		sf::Sprite testSprite;
		testSprite.setTexture(m_tileTextures[tileTextureNo(hand[i])]);
		testSprite.setPosition(sf::Vector2f(100 + 42 * i, 526));
		window.draw(testSprite);
	}

	// Render the drawn tile (still testing)
	if (drawnTile != NUM_OF_TILES) {
		sf::Sprite testSprite;
		testSprite.setTexture(m_tileTextures[tileTextureNo(drawnTile)]);
		testSprite.setPosition(sf::Vector2f(110 + 42 * 14, 526));
		window.draw(testSprite);
	}
}

void Client::discard(int i) {
	if (i != 14 && drawnTile != NUM_OF_TILES) {
		hand[i] = drawnTile;
		std::sort(std::begin(hand), std::end(hand));
	}
	drawnTile = NUM_OF_TILES;
}
