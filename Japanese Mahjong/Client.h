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
#include <vector>
#include <memory>

#include "../Tile.h"
#include "../MPacket.h"
#include "../Match.h"

const std::string FONT = "sazanami-gothic.ttf";

class Client {
public:
	Client(sf::TcpSocket& server, int NUMPLAYERS, sf::String usernames[], sf::Int8 playerNo); // Constructor
	void run(); // Order the client to run the game
private:
	// Private methods
	void reportLoadFailure(); // Called if Client ever fails an asset load
	void loadTileTextures(); // Loads textures for the tiles
	int tileTextureNo(Tile tile); // Gets the index of the texture corresponding to the tile
	void receiveInformation(); // Receive and process information from the game server
	void getFirstHand(sf::Packet& packet); // Extracts the first hand of the round from the packet
	void getDraw(sf::Packet& packet); // Extracts the draw from the packet
	void render(sf::RenderWindow& window); // Renders the 2D state of the world to the window in preparation for display
	void discard(int i); // Discard the ith tile from the hand (i = 14 is the draw instead)

	// Private members
	sf::TcpSocket& m_server;
	int m_NUMPLAYERS;
	sf::String* m_usernames;
	sf::Int8 m_playerNo;
	sf::Font m_font;
	sf::Texture m_tileTextures[NUM_OF_TILE_TEXTURES];
	sf::Texture m_facedownTileTexture;

	std::unique_ptr<Match> match;
	Tile m_hand[13];
	Tile m_drawnTile = NUM_OF_TILES; // Defaults to error/no drawn tile
	bool m_inputAllowed = false; // Players don't always have actions available to them - this is coordinated by the server

	// Private constant members
	unsigned int m_width = 800;
	unsigned int m_height = 600;
};
#endif