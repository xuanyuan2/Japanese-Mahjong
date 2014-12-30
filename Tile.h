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

enum TileTypes {
	PIN_1,
	PIN_2,
	PIN_3,
	PIN_4,
	PIN_5,
	PIN_6,
	PIN_7,
	PIN_8,
	PIN_9,
	SOU_1,
	SOU_2,
	SOU_3,
	SOU_4,
	SOU_5,
	SOU_6,
	SOU_7,
	SOU_8,
	SOU_9,
	WAN_1,
	WAN_2,
	WAN_3,
	WAN_4,
	WAN_5,
	WAN_6,
	WAN_7,
	WAN_8,
	WAN_9,
	WIND_E,
	WIND_S,
	WIND_W,
	WIND_N,
	DRAG_W,
	DRAG_G,
	DRAG_R
};

const sf::Uint8 PIN_5_RED = PIN_5 * 4 + 3;
const sf::Uint8 SOU_5_RED = SOU_5 * 4 + 3;
const sf::Uint8 WAN_5_RED = WAN_5 * 4 + 3;

const int NUM_OF_TILE_TEXTURES = 34 + 3; // 34 types of tiles, plus three red dora at the end

// Helper functions to identify tiles
// Precondition: Paramter tile is a valid tile (passes isValidTile)

inline sf::Int16 isValidTile(sf::Uint8 tile) {
	return (tile >= PIN_1 * 4)  // First tile
		&& (tile < (DRAG_R + 1) * 4); // Last tile
}

inline int typeOf(sf::Uint8 tile) {
	return (sf::Int16)floor((int)tile / 4);
}

inline bool isNumber(sf::Uint8 tile) {
	return tile < (WIND_E * 4); // First nonnumber tile
}

inline bool isPin(sf::Uint8 tile) {
	return tile < (SOU_1 * 4); // First non-pin tile
}

inline bool isSou(sf::Uint8 tile) {
	return (tile >= (SOU_1 * 4)) // First sou tile
		&& (tile < (WAN_1 * 4)); // First non-sou tile
}

inline bool isWan(sf::Uint8 tile) {
	return (tile >= (WAN_1 * 4)) // First wan tile
		&& (tile < (WIND_E * 4)); // First non-wan tile
}

inline bool isWind(sf::Uint8 tile) {
	return (tile >= (WIND_E * 4)) // First wind tile
		&& (tile < (DRAG_W * 4)); // First non-wind tile
}

inline bool isDragon(sf::Uint8 tile) {
	return (tile >= (DRAG_W * 4)); // First dragon tile
}

inline bool isRed(sf::Uint8 tile) {
	return (isNumber(tile) && // Tile must be a number
		(typeOf(tile) % 9) == 4 && // Tile must be 5
		(tile % 4) == 3); // Tile is the last of its type
}

inline sf::String filename(sf::Uint8 tile) {
	sf::String filepath = "tiles/";
	filepath += std::to_string((int)typeOf(tile));
	if (isRed(tile)) filepath += "_red";
	filepath += ".png";
	return filepath;
}

#endif