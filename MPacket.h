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

This file handles the packaging of information sent and received between
the server and the client into Mahjong Packets ("MPacket").
*/

#include <SFML/Network.hpp>

#include "../Tile.h"

const int NUMPLAYERS = 4;
const int MAXNUMOFTILES = 14; // Only dealers ever get this many tiles (at the start)

enum MPacketHeader : sf::Int8 { 
	INITIAL_INFO, // The initial information all players receive
	STATE_UPDATE, // This packet contains information about players
	FIRST_HAND, // Indicates this packet contains the player's first of tiles
	DRAW, // Server sending player information on draw
	DISCARD_SELF, // Player informing server of his discard choice
	DISCARD // The server reports other players' discards
};

class MPacket { // Base class of all Mpackets
public:
	sf::Int8 getHeader() { return m_header; } // Return this packet's header
protected:
	MPacket() {} // Protected constructor to provent instantiation of this base class
	sf::Int8 m_header; // Contains information on the MPacket type
};

class InitPacket : public MPacket {
public:
	InitPacket() { // TODO: Expand construtor to accept initialization info
		m_header = INITIAL_INFO;
	};
private:
	// Initialization info goes here
};

class StatePacket : public MPacket {
public:
	StatePacket() {
		m_header = STATE_UPDATE;
	}
private:
	// State update info goes here
};

class FirstHandPacket : public MPacket {
public:
	FirstHandPacket() {
		m_header = FIRST_HAND;
	}
	const Tile* getHand() { return hand; }
private:
	Tile hand[13];
};

class DrawPacket : public MPacket {
public:
	DrawPacket() {
		m_header = DRAW;
	}
	const Tile getDraw() { return draw; }
private:
	Tile draw;
};

class DiscardSelfPacket : public MPacket {
public:
	DiscardSelfPacket() {
		m_header = DISCARD_SELF;
	}
	const Tile getDiscard() { return discard; }
private:
	Tile discard;
};

class DiscardPacket : public MPacket {
public:
	DiscardPacket() {
		m_header = DISCARD;
	}
	const Tile getDiscard() { return discard; }
private:
	Tile discard;
};

/*
// SFML Packet << and >> overload to support MPackets
sf::Packet& operator <<(sf::Packet& packet, const MPacket& mpacket) {
	for (int i = 0; i < NUMPLAYERS; i++) {
		packet << mpacket.scores[i];
	}
	for (int j = 0; j < MAXNUMOFTILES; j++) {
		packet << mpacket.tiles[j];
	}
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, MPacket& mpacket) {
	for (int i = 0; i < NUMPLAYERS; i++) {
		packet >> mpacket.scores[i];
	}
	for (int j = 0; j < MAXNUMOFTILES; j++) {
		packet >> mpacket.tiles[j];
	}
	return packet;
}
*/