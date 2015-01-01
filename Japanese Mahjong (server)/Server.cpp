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

This file handles the operation of the server.
*/

#include <iostream>
#include <algorithm> // For shuffling

#include "Server.h"
#include "../MPacket.h"

// Server constructor
Server::Server(sf::TcpSocket clients[], int NUMPLAYERS, sf::String usernames[]) {
	m_clients = clients;
	m_NUMPLAYERS = NUMPLAYERS;
	m_usernames = usernames;

	std::random_device rd;
	rng.reset(new std::mt19937(rd()));

	// Player hands
	for (int i = 0; i < NUMPLAYERS; ++i) {
		std::vector<Tile> hand;
		playerHands.push_back(hand);
	}
	// Player discards
	for (int i = 0; i < NUMPLAYERS; ++i) {
		std::vector<Tile> discards;
		playerDiscards.push_back(discards);
	}
}

void Server::run() {
	std::cout << "Game launched." << std::endl;
	determineSeating();

	redistributeTiles();

	// The following chunk of code is for debugging
	for (int p = 0; p < m_NUMPLAYERS; ++p) {
		std::cout << "Player " << p + 1 << std::endl;
		for (int t = 0; t < 13; t++) {
			std::cout << (int)playerHands[p][t] << std::endl;
		}

		FirstHandPacket hand(playerHands[p]);
		sf::Packet packet;
		packet << hand;
		m_clients[p].send(packet);

		DrawPacket draw(drawTile());
		packet.clear();
		packet << draw;
		m_clients[p].send(packet);
	}

	while (true) {} // Loop to keep the console open for debugging
}

void Server::determineSeating() {
	/* Initial seating order (i.e. Players 1-4) was determined by server from first to 
	 * last to connect. There are complex methods in real mahjong to determine order 
	 * of play and seat wind distribution, but this can be vastly simplified by
	 * simply letting an RNG randomly determine who is dealer and determining winds
	 * from there.
	 */

	std::uniform_int_distribution<int> dist(0, m_NUMPLAYERS);
	m_dealer = dist(*rng); // The dealer of a hand is always in east position that hand
}

void Server::redistributeTiles() {
	playerHands.clear();
	playerDiscards.clear();
	liveWall.clear();
	deadWall.clear();

	int playerHandMaxSize = 13; // Max size when hands are first made, that is
	int deadWallMaxSize = 14;
	int liveWallMaxSize = 70;

	// Shuffle all tiles 
	Tile tiles[NUM_OF_TILES];
	for (Tile i = 0; i < NUM_OF_TILES; ++i)
		tiles[i] = i;
	std::shuffle(std::begin(tiles), std::end(tiles), *rng);
	
	// Fill the live wall
	for (int j = 0; j < liveWallMaxSize; ++j)
		liveWall.push_back(tiles[j]);

	// Fill the dead wall
	for (int k = liveWallMaxSize; k < liveWallMaxSize + deadWallMaxSize; ++k)
		deadWall.push_back(tiles[k]);

	// Fill out players' hands
	// Note - if there are less than 4 players, this code will not distribute all 136 tiles.
	// Not an issue for normal mahjong, but if 3 player is implemented, this must be fixed
	for (int p = 0; p < m_NUMPLAYERS; ++p) {
		for (int t = liveWallMaxSize + deadWallMaxSize; t < NUM_OF_TILES; t++) {
			playerHands[p].push_back(tiles[p * playerHandMaxSize + t]);
		}
	}
}

Tile Server::drawTile() {
	if (liveWall.empty())
		return NUM_OF_TILES;
	Tile draw = liveWall.back();
	liveWall.pop_back();
	return draw;
}
