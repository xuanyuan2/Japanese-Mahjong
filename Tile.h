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

This file defines the tiles used during play.
*/

#ifndef TILE_H
#define TILE_H
#include <SFML/Network.hpp>

enum SUITS : sf::Int8 {
	PIN, // Also known as dots or circles
	SOU, // Also known as bamboo
	WAN, // Also known as characters
	WIND,
	DRAGON
};

enum NUMBERS : sf::Int8 { // May seem redundant, but helps with off-by-one
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	NaN // Not a Number
};

enum WINDS : sf::Int8 {
	EAST,
	SOUTH,
	WEST,
	NORTH,
	NaW // Not a Wind
};

enum DRAGONS : sf::Int8 {
	WHITE,
	GREEN,
	RED,
	NaD // Not a Dragon
};

struct Tile {
	// Basic constructor for convenience
	Tile(SUITS psuit, NUMBERS pnumber, WINDS pwind, DRAGONS pdragon) :
		suit(psuit),
		number(pnumber),
		wind(pwind),
		dragon(pdragon)
	{	}
	Tile() {} // Default constructor does nothing
	sf::Int8 suit;
	sf::Int8 number;
	sf::Int8 wind;
	sf::Int8 dragon;

	/* The following overloads are defined as friends of Tile. This is rather inelegant!
	 * However, it allows the functions to be defined in Tile.h without encountering
	 * duplicate definition errors with the linker.
	 * Normally, you would work around this by simply defining these overloads in Tile.cpp,
	 * but "Tile" is meant to be shared by two projects, requiring the even less elegant
	 * solution of compiling Tile.cpp in both projects.
	 */

	// SFML Packet << overload to support tiles
	friend sf::Packet& operator<<(sf::Packet& packet, const Tile& tile) {
		return packet << tile.suit << tile.number << tile.wind << tile.dragon;
	}

	// SFML Packet >> overload to support tiles
	friend sf::Packet& operator>>(sf::Packet& packet, Tile& tile) {
		return packet >> tile.suit >> tile.number >> tile.wind >> tile.dragon;
	}
};

#endif