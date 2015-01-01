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

#include <SFML/Network.hpp>
#include <random> // For RNG
#include <vector>

#include "../Tile.h"

class Server {
public:
	Server(sf::TcpSocket clients[], int NUMPLAYERS, sf::String usernames[]);
	~Server(); // Destructor
	void run(); // Order the server to run the game
private:
	// Private Methods
	void determineSeating(); // Decide which player starts with what wind and who is dealer
	void redistributeTiles(); // Takes back all tiles and distributes them to players and walls

	// Private variables
	sf::TcpSocket* m_clients; // Array of sockets connected to clients
	int m_NUMPLAYERS;
	sf::String* m_usernames;
	std::mt19937* rng;
	int dealer; // The player who is currently dealer (and hence east)
	int hand = 0; // There are four or more of these every round
	int round = 0; // Japanese Mahjong is typically played with two rounds (han-chan)
	std::vector< std::vector<Tile> > playerHands, playerDiscards;
	std::vector<Tile> liveWall, deadWall;
};
