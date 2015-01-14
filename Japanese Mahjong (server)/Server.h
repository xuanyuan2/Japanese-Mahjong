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
#include <memory>

#include "../Tile.h"
#include "../Match.h"

class Server {
public:
	Server(sf::TcpSocket clients[], int NUMPLAYERS, sf::String usernames[]);
	void run(); // Order the server to run the game
private:
	// Private Methods
	sf::Int8 determineDealer(); // Decide which player starts with what wind and who is dealer
	void redistributeTiles(); // Takes back all tiles and distributes them to players and walls
	void transmitHands(); // Transmits initial hands to all players
	bool handOver(); // Checks to see if the hand is over (no tiles left to draw, a player has won, etc.)
	Tile drawTile(); // Returns a tile from the live wall, or NUM_OF_TILES if the live wall is empty
	// At the end of a hand, returns the changes to each players' score
	// Updates the repeat parameter based on whether or not the hand should be "repeated",
	// with respect to the 4 hand per round limit
	std::vector<int> scoreChanges(bool& repeat); 
	void transmitMatchUpdate(std::vector<int> scoreChanges, bool repeat); // Update clients' match info

	// Private variables
	sf::TcpSocket* m_clients; // Array of sockets connected to clients
	int m_NUMPLAYERS;
	sf::String* m_usernames;
	std::unique_ptr<std::mt19937> rng;
	std::vector< std::vector<Tile> > playerHands, playerDiscards;
	std::vector<Tile> liveWall, deadWall;
	std::unique_ptr<Match> match;
};
