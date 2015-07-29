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

	handleDealership();

	// Central game loop - game only continues so long as the match is actually ongoing
	// Each iteration of this loop covers one hand
	while (match->isActive()) {
		redistributeTiles();
		transmitHands();

		// Most of gameplay - drawing, discarding, melding - occurs in this loop
		// Each iteration of this while loop is one turn
		while (!handOver()) {
			// For now, mostly test code follows until the end of the loop

			int currentPlayer = match->getCurrentPlayer();
			std::cerr << "Player this turn: " << currentPlayer + 1 << std::endl;
			std::cerr << "Tiles left: " << match->getNumTilesLeft() << std::endl;

			// Draw a tile for current player
			sf::Packet packet;
			Tile drawnTile = drawTile();
			std::cerr << "Client drew: " << (int)drawnTile << std::endl;

			// Add tile to current player's hand
			playerHands[currentPlayer].push_back(drawnTile);

			// Send tile to current player's client
			DrawPacket draw(drawnTile);
			packet << draw;
			m_clients[currentPlayer].send(packet);

			// Get the player's discard
			sf::Uint8 header = -1; // Initialized as invalid header
			Tile discard;
			while (header != SELF_DISCARD) { // Ignore all player input until they send a discard
				packet.clear();
				m_clients[currentPlayer].receive(packet);
				packet >> header;
				if (header == SELF_DISCARD) {
					SelfDiscardPacket discardPacket;
					packet >> discardPacket;
					discard = discardPacket.getDiscard();
				}
			}

			// Process tile movement from hand to discard
			std::cerr << "Client attempted to discard: " << (int)discard << std::endl;
			auto iterator = std::find(std::begin(playerHands[currentPlayer]), std::end(playerHands[currentPlayer]), discard);
			if (iterator != std::end(playerHands[currentPlayer])) { // Check and make sure the client send a valid discard!
				playerHands[currentPlayer].erase(iterator);
			}
			else { // Something's gone horribly wrong
				std::cerr << "A player sent an invalid discard" << std::endl; 
				int i = 0;
				for (auto it = std::begin(playerHands[currentPlayer]); it != std::end(playerHands[currentPlayer]); ++it) {
					std::cerr << (int)playerHands[currentPlayer][i] << std::endl;
					++i;
				}
				while (true) {} // Infinite loop for debug
			}
			playerDiscards[currentPlayer].push_back(discard); // Add to discard heap
		
			// TODO: Check if other players can chii/pon/ron off discard

			// Send discard to other players
			std::cerr << "Client succesfully discarded " << int(discard) << std::endl;
			DiscardPacket dp(discard);
			packet.clear();
			packet << dp;
			for (int i = 0; i < NUM_PLAYERS; ++i) {
				if (i != currentPlayer)
					m_clients[i].send(packet);
			}

			// Advance to next turn
			match->nextTurn();
		}

		// The hand has ended
		bool repeat = false;
		std::vector<int> scoreUpdate = scoreChanges(repeat);
		transmitMatchUpdate(scoreUpdate, repeat);
		match->update(scoreUpdate, repeat);
	}
	
}

void Server::handleDealership() {
	/* Initial seating order (i.e. Players 1-4) was determined by server from first to 
	 * last to connect. There are complex methods in real mahjong to determine order 
	 * of play and seat wind distribution, but this can be vastly simplified by
	 * simply letting an RNG randomly determine who is dealer. Player seat wind can
	 * then be determined from there (this is handled by the match object).
	 */

	std::uniform_int_distribution<int> dist(0, m_NUMPLAYERS);
	int dealer = dist(*rng); 
	match.reset(new Match(dealer));
	// Relay the information to all players
	sf::Packet dealerPacket;
	for (int p = 0; p < m_NUMPLAYERS; ++p) {
		dealerPacket << dealer;
		m_clients[p].send(dealerPacket);
	}
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
	int wallMaxSize = liveWallMaxSize + deadWallMaxSize;
	for (int p = 0; p < m_NUMPLAYERS; ++p) {
		for (int t = wallMaxSize + p * playerHandMaxSize; t < wallMaxSize + (p + 1) * playerHandMaxSize; t++) {
			playerHands[p].push_back(tiles[t]);
		}
		std::sort(std::begin(playerHands[p]), std::end(playerHands[p]));
	}
}

void Server::transmitHands() {
	for (int p = 0; p < m_NUMPLAYERS; ++p) {
		FirstHandPacket hand(playerHands[p]);
		sf::Packet packet;
		packet << hand;
		m_clients[p].send(packet);
	}
}

bool Server::handOver() {
	if (liveWall.size() < 0) return true; // There are no tiles left to draw
	return false; // TODO: Replace with code checking for victory
}

std::vector<int> Server::scoreChanges(bool& repeat) {
	std::vector<int> output;
	// TODO: Actually score hands
	for (int i = 0; i < m_NUMPLAYERS; i++) {
		output.push_back(0);
	}
	// TODO: Actually check if hand should repeat
	repeat = false;
	return output;
}

void Server::transmitMatchUpdate(std::vector<int> scoreChanges, bool repeat) {
	for (int p = 0; p < m_NUMPLAYERS; ++p) {
		sf::Packet packet;
		
		m_clients[p].send(packet);
	}
}

Tile Server::drawTile() {
	if (liveWall.empty())
		return NUM_OF_TILES;
	Tile draw = liveWall.back();
	liveWall.pop_back();
	return draw;
}
