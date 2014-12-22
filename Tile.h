/*
Designed by Xuan Yuan Huang, a desperate UCLA undergrad CS student.
Copyright 2014 Xuan Yuan Huang

Built with the SFML 2.1 Engine.
SFML 2.1 is licensed under zlib/libpng. 

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

Developed in Visual Studio 2012 (thanks for the free copy, MS Dreamspark!).
Visual Studio is, of course, Microsoft's proprietary IDE.

This file defines the tiles used during play.
*/

#ifndef TILE_H
#define TILE_H
#include <SFML/Network.hpp>

enum class SUIT : sf::Int8 {
	PIN, // Also known as dots or circles
	SOU, // Also known as bamboo
	WAN, // Also known as characters
	WIND,
	DRAGON
};

enum class NUMBERS : sf::Int8 { // To avoid off-by-one errors
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE
};

enum class WIND_TYPE : sf::Int8 {
	EAST,
	SOUTH,
	WEST,
	NORTH
};

enum class DRAGON_TYPE : sf::Int8 {
	WHITE,
	GREEN,
	RED
};

struct Tile {
	sf::Int8 suit;
	sf::Int8 wind;
	sf::Int8 dragon;
	sf::Int8 number;
};

// SFML Packet << and >> overload to support tiles
sf::Packet& operator <<(sf::Packet& packet, const Tile& tile) {
	return packet << tile.suit << tile.wind << tile.dragon << tile.number;
}

sf::Packet& operator >>(sf::Packet& packet, Tile& tile) {
	return packet >> tile.suit >> tile.wind >> tile.dragon >> tile.number;
}
#endif