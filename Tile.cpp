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

#include <math.h>

// Inspired by the tile data structure by github user "henryyang42"
/* Japanese Mahjong typically has 136 tiles - 4 for each of 34 different types:
* 108 numbered tiles (1-9 inclusive) are divided into three suits - pin (dots),
*   sou (bamboo), and wan (characters) - for a total of 27 kinds of tile
* 16 wind tiles (east, south, west, and north)
* 12 dragon tiles (white, green, and red)
*
* Each tile in the game can thus be assigned a unique number from 0 to 135 inclusive.
* Each KIND of tile can be assigned a unique number from 0 to 34 inclusive.
* The tiles are grouped by type in order, so that for any type of tile x in [0,34],
* its four copies can be found in [0,135] at 4x, 4x+1, 4x+2, and 4x+3.
*
* In this version of the game, there are also three red "dora" - one single specially
* colored number 5 tile per suit that boosts the value of a winning hand.
* This red tile is stored as the last of the four of its type.
*/

// Helper functions to identify tiles

sf::Int8 isValidTile(sf::Int8 tile) {
	return (tile >= PIN_1 * 4)  // First tile
		&& (tile < (DRAG_R + 1) * 4); // Last tile
}

sf::Int8 typeOf(sf::Int8 tile) {
	return (sf::Int8)floor((int)tile / 4);
}

bool isNumber(sf::Int8 tile) {
	return tile < 108;
}

bool isPin(sf::Int8 tile) {
	return tile < 36;
}

bool isSou(sf::Int8 tile) {
	return (tile >= 36) && (tile < 72);
}

bool isWan(sf::Int8 tile) {
	return (tile >= 72) && (tile < 108);
}

bool isWind(sf::Int8 tile) {
	return (tile >= 108) && (tile < 124);
}

bool isDragon(sf::Int8 tile) {
	return (tile >= 124) && (tile < 136);
}

bool isRed(sf::Int8 tile) {
	return (isNumber(tile) && // Tile must be a number
		(typeOf(tile) % 9) == 4 && // Tile must be 5
		(tile % 4) == 3); // Tile is the last of its type
}
